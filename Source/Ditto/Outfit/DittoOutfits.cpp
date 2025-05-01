// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "DittoOutfits.h"

#include "BlueprintGameplayTagLibrary.h"
#include "DittoFragment_OutfitPart.h"
#include "DittoOutfitAvatar.h"
#include "Combee/Core/Item/CombeeItemInterface.h"
#include "Combee/Helpers/CombeeLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UDittoOutfits::UDittoOutfits()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = false;

    // ...

    HiveReplicateCondition = COND_SimulatedOnly;
}

bool UDittoOutfits::CheckItemCompatible_Implementation(const TScriptInterface<ICombeeItemInterface>& Item,
                                                       const int32& Index) const
{
    const auto Fragment = Item->FindFragmentByClass<UDittoFragment_OutfitPart>();

    if (!(Fragment && UCombeeLibrary::IsNativeHost(Item, Fragment)))
    {
        return false;
    }

    return CheckPartTypeCompatible(Fragment->Part, Index);
}


// Called when the game starts
void UDittoOutfits::BeginPlay()
{
    // Skip Default Container Init Behaviour
    Super::Super::BeginPlay();

    // ...
}

void UDittoOutfits::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    FDoRepLifetimeParams Params;
    Params.bIsPushBased = true;
    Params.Condition = COND_None;
    DOREPLIFETIME_WITH_PARAMS(ThisClass, Layout, Params);
    DOREPLIFETIME_WITH_PARAMS(ThisClass, Avatar, Params);
}

void UDittoOutfits::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
    Super::PreReplication(ChangedPropertyTracker);
}

void UDittoOutfits::OnInitialization_Implementation()
{
    if (GetOwner()->HasAuthority())
    {
        this->PostCellChange.AddUniqueDynamic(this, &ThisClass::HandleAuthorityCellMutation);
    }
    this->ReceiveContainerMutation.
          AddDynamic(this, &ThisClass::UDittoOutfits::HandleOutfitsContainerMutation);
    Super::OnInitialization_Implementation();
}

void UDittoOutfits::SetAvatar(AActor* InAvatar)
{
    if (GetOwner()->HasAuthority() && InAvatar != Avatar)
    {
        ResetLayout();

        if (IsValid(InAvatar) && InAvatar->Implements<UDittoOutfitAvatar>())
        {
            COMPARE_ASSIGN_AND_MARK_PROPERTY_DIRTY(ThisClass, Avatar, InAvatar, this);
            IDittoOutfitAvatar::Execute_SetupOutfitAvatar(Avatar, this);
        }
        else
        {
            COMPARE_ASSIGN_AND_MARK_PROPERTY_DIRTY(ThisClass, Avatar, nullptr, this);
        }
    }
}

void UDittoOutfits::OnRep_Avatar_Implementation()
{
}

void UDittoOutfits::OnReq_Layout_Implementation()
{
}

FDittoOutfitLayoutPart UDittoOutfits::RegisterPart(const FGameplayTagContainer Part, const FInstancedStruct& PartData)
{
    const auto Info = this->Layout.Register(Part, PartData);
    MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, Layout, this);
    this->Hive.Cells.SetNum(Layout.PartsRegistry.Num());
    this->Hive.Cells.Last().Init(this, Info.ContainerIndex);
    return Info;
}

void UDittoOutfits::ResetLayout()
{
    Layout.Reset();
    this->Hive.Cells.SetNum(Layout.PartsRegistry.Num());
}

FDittoOutfitLayoutPart UDittoOutfits::RetrievePartInfo(const int32 Index) const
{
    if (Layout.PartsRegistry.IsValidIndex(Index))
    {
        return *Layout.PartsRegistry[Index].GetPtr<FDittoOutfitLayoutPart>();
    }
    return {};
}

void UDittoOutfits::FindIndexByPart(const FGameplayTagContainer& Part, TArray<int32>& OutIndices, int32& First,
                                    int32& Last, bool& Found) const
{
    First = INDEX_NONE;
    Last = INDEX_NONE;
    OutIndices.Empty();

    for (int Index = 0; Index < Layout.PartsRegistry.Num(); ++Index)
    {
        const auto PartInfo = Layout.PartsRegistry[Index];
        if (CheckPartTypeCompatible(Part, Index))
        {
            const int32 ContainerIndex = PartInfo.GetPtr()->ContainerIndex;
            check(CheckIndexIsValid(ContainerIndex))
            OutIndices.Add(ContainerIndex);
        }
    }

    Found = OutIndices.Num() > 0;
    if (Found)
    {
        First = OutIndices[0];
        Last = OutIndices.Last();
    }
}

bool UDittoOutfits::CheckPartTypeCompatible(const FGameplayTagContainer& PartType, const int32& Index) const
{
    const auto PartInfo = Layout.PartsRegistry[Index].GetPtr();
    if (!PartInfo || !PartType.IsValid() || !PartInfo->IsValid())
    {
        return false;
    }

    return UBlueprintGameplayTagLibrary::HasAllTags(PartType, PartInfo->Part, true);
}

void UDittoOutfits::HandleAuthorityCellMutation(const FCombeeCellMutationContext& Context)
{
    ensure(Context.State == ECombeeExecutionState::Success);
    {
        if (IsValid(Avatar) && Avatar->Implements<UDittoOutfitAvatar>())
        {
            const auto Handled = IDittoOutfitAvatar::Execute_ReceiveOutfitUpdate(Avatar, this, Context);
            if (!Handled)
            {
                ProcessAvatarTakeOff(Context);
            }
        }
    }
}

void UDittoOutfits::HandleOutfitsContainerMutation(const FCombeeContainerMutationContext& Context)
{
    if (IsValid(Avatar) && Avatar->Implements<UDittoOutfitAvatar>())
    {
        const auto Handled = IDittoOutfitAvatar::Execute_ReceiveOutfitUpdateDeferred(Avatar, this, Context);
        if (!Handled)
        {
            ProcessAvatarWear(Context);
        }
    }
}

void UDittoOutfits::ProcessAvatarWear_Implementation(const FCombeeContainerMutationContext& Context)
{
    if (Context.MutationType == ECombeeMutationType::Change)
    {
        for (auto&& Index : Context.Indices)
        {
            bool bIndexValid;
            const auto OutfitItem = GetCell(Index, bIndexValid).Item;

            ensure(bIndexValid);

            const auto PartInfo = RetrievePartInfo(Index);
            if (!PartInfo.IsValid())
            {
                continue;
            }

            if (OutfitItem)
            {
                OutfitItem->FindFragmentByClass<UDittoFragment_OutfitPart>()->Wear(PartInfo.PartData);
            }
        }
    }
}

void UDittoOutfits::TakeOff_Implementation(const TSubclassOf<UDittoFragment_OutfitPart> FragmentClass,
                                           const FInstancedStruct& PartData)
{
    FragmentClass.GetDefaultObject()->TakeOff(PartData);
}

void UDittoOutfits::ProcessAvatarTakeOff_Implementation(const FCombeeCellMutationContext& Context)
{
    const auto PartInfo = RetrievePartInfo(Context.TargetIndex);
    if (!PartInfo.IsValid())
    {
        return;
    }

    const auto PreviousItem = Context.PreviousInstance;
    if (PreviousItem)
    {
        // Previous Item May not be replicated to Client
        TakeOff(PreviousItem->FindFragmentByClass<UDittoFragment_OutfitPart>()->GetClass(), PartInfo.PartData);
    }
}
