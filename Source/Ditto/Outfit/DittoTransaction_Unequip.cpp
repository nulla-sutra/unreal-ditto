// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "DittoTransaction_Unequip.h"

#include "DittoOutfits.h"
#include "CombeePresets/Common/Transactions/CombeeTransaction_Move.h"

void UDittoTransaction_Unequip::OnExecute_Implementation()
{
    const auto PayloadPtr = Payload.GetPtr<FPayloadType>();

    if (!PayloadPtr)
    {
        MARK_TRANSACTION_FAILED_AND_RETURN();
    }

    const auto OutfitContainer = PayloadPtr->OutfitContainer;
    const auto OutfitIndex = PayloadPtr->OutfitIndex;
    const auto ClosetContainer = PayloadPtr->OtherContainer;

    if (!IsValid(OutfitContainer) || !IsValid(ClosetContainer))
    {
        MARK_TRANSACTION_FAILED_AND_RETURN();
    }

    if (!OutfitContainer->CheckIndexIsValid(OutfitIndex))
    {
        MARK_TRANSACTION_FAILED_AND_RETURN();
    }


    FCombeeTransactionPayload_Move AppendPayload{.bAutoExpand = false};

    AppendPayload.FromContainer = OutfitContainer;
    AppendPayload.FromIndex = OutfitIndex;
    AppendPayload.TargetContainer = ClosetContainer;

    ProcessTransaction<UCombeeTransaction_Move>(
        FInstancedStruct::Make(AppendPayload));
}
