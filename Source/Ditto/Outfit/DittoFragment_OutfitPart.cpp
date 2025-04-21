// Fill out your copyright notice in the Description page of Project Settings.


#include "DittoFragment_OutfitPart.h"

#include "DittoOutfits.h"
#include "DittoTransaction_Equip.h"
#include "DittoTransaction_Unequip.h"
#include "Combee/Core/Container/CombeeContainer.h"
#include "CombeeTransaction/Subsystem/CombeeTransactionSubsystem.h"
#include "CombeeUse/Usable/CombeeUsableTypes.h"

FVoidCoroutine UDittoFragment_OutfitPart::OnUse(AController* const& Instigator,
                                                TInstancedStruct<FCombeeUsePayload>& Payload,
                                                bool& bSuccess,
                                                const UE5Coro::TLatentContext<> _)
{
    const auto PayloadPtr = Payload.GetPtr();

    if (!PayloadPtr)
    {
        bSuccess = false;
        co_return ;
    }
    const auto Subsystem = GetWorld()->GetGameInstance()->GetSubsystem<UCombeeTransactionSubsystem>();

    const auto TriggerContainer = PayloadPtr->TriggerContainer;
    const auto TriggerIndex = PayloadPtr->TriggerIndex;

    if (!IsValid(Instigator) || !IsValid(TriggerContainer) || !IsValid(Subsystem))
    {
        bSuccess = false;
        co_return ;
    }

    bool bTriggerIndexValid;
    const auto TriggerCell = TriggerContainer->GetCell(TriggerIndex, bTriggerIndexValid);
    bool bItemStillValid = bTriggerIndexValid && TriggerCell.Item == GetHostItem();

    if (!bItemStillValid)
    {
        bSuccess = false;
        co_return;
    }

    auto* OutfitContainer = Cast<UDittoOutfits>(TriggerContainer);

    if (OutfitContainer)
    {
        int8 ClosetIndex = 0;
        UCombeeContainer* Closet = nullptr;
        while (bItemStillValid && ClosetIndex < Closets.Num())
        {
            const auto CellInfo = OutfitContainer->GetCell(TriggerIndex, bTriggerIndexValid);
            bItemStillValid = CellInfo.Item == GetHostItem();
            if (bItemStillValid)
            {
                const auto ClosetClass = Closets[ClosetIndex];

                Closet = Cast<UCombeeContainer>(
                    OutfitContainer->GetOwner()->GetComponentByClass(ClosetClass));
                if (IsValid(Closet))
                {
                    break;
                }

                ClosetIndex++;
            }
        }

        if (!IsValid(Closet))
        {
            bSuccess = false;
            co_return;
        }

        FDittoOutfitTransactionPayload_Unequip UnequipPayload;
        UnequipPayload.Part = Part;
        UnequipPayload.OutfitContainer = OutfitContainer;
        UnequipPayload.OutfitIndex = TriggerIndex;
        UnequipPayload.OtherContainer = Closet;

        FCombeeTransactionResponse Response;
        co_await UE5Coro::Latent::ChainEx(&UCombeeTransactionSubsystem::RequestTransactionLatent,
                                          Subsystem,
                                          Instigator,
                                          UDittoTransaction_Unequip::StaticClass(),
                                          FInstancedStruct::Make(UnequipPayload),
                                          std::ref(Response),
                                          std::placeholders::_2);
        bSuccess &= Response.State == ECombeeExecutionState::Success;
        co_return ;
    }
    else
    {
        OutfitContainer = TriggerContainer->GetOwner()->GetComponentByClass<UDittoOutfits>();

        if (!IsValid(OutfitContainer))
        {
            bSuccess = false;
            co_return;
        }
        FDittoOutfitTransactionPayload_Equip EquipPayload;
        EquipPayload.bPreferEmpty = true;
        EquipPayload.Part = Part;
        EquipPayload.OutfitContainer = OutfitContainer;
        EquipPayload.OtherContainer = TriggerContainer;
        EquipPayload.OtherIndex = TriggerIndex;

        FCombeeTransactionResponse Response;

        co_await UE5Coro::Latent::ChainEx(&UCombeeTransactionSubsystem::RequestTransactionLatent, Subsystem,
                                          Instigator,
                                          UDittoTransaction_Equip::StaticClass(),
                                          FInstancedStruct::Make(EquipPayload),
                                          std::ref(Response),
                                          std::placeholders::_2);
        bSuccess &= Response.State == ECombeeExecutionState::Success;
        co_return ;
    }
}

void UDittoFragment_OutfitPart::TakeOff_Implementation(const FInstancedStruct& PartData)
{
    unimplemented();
}

void UDittoFragment_OutfitPart::K2_TakeOff_Implementation(const FInstancedStruct& PartData)
{
    unimplemented();
}

void UDittoFragment_OutfitPart::Wear_Implementation(const FInstancedStruct& PartData)
{
    unimplemented();
}

void UDittoFragment_OutfitPart::K2_Wear_Implementation(const FInstancedStruct& PartData)
{
    unimplemented();
}
