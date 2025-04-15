// Fill out your copyright notice in the Description page of Project Settings.


#include "DittoOutfits.h"

#include "BlueprintGameplayTagLibrary.h"
#include "DittoOutfitAvatar.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UDittoOutfits::UDittoOutfits()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UDittoOutfits::CheckItemCompatible_Implementation(const TScriptInterface<ICombeeItemInterface>& Item,
                                                       const int32& Index) const
{
	return Super::CheckItemCompatible_Implementation(Item, Index);
}


// Called when the game starts
void UDittoOutfits::BeginPlay()
{
	if (GetOwner()->HasAuthority())
	{
		this->PostCellChange.AddDynamic(this, &ThisClass::HandleAuthorityCellMutation);
	}
	Super::Super::BeginPlay();

	// ...
}

void UDittoOutfits::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS(ThisClass, Layout, Params);
	DOREPLIFETIME_WITH_PARAMS(ThisClass, Avatar, Params);
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

FDittoOutfitLayoutPart UDittoOutfits::RegisterPart(FGameplayTagContainer Part, const FInstancedStruct& PartData)
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
		if (UBlueprintGameplayTagLibrary::HasAllTags(PartInfo.GetPtr()->Part, Part, true))
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

void UDittoOutfits::HandleAuthorityCellMutation(const FCombeeCellMutationContext& Context)
{
	if (Context.State == ECombeeExecutionState::Success)
	{
		if (IsValid(Avatar) && Avatar->Implements<UDittoOutfitAvatar>())
		{
			IDittoOutfitAvatar::Execute_ReceiveOutfitUpdate(Avatar, this, Context);
		}
	}
}
