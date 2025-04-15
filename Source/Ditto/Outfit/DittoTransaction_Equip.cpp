// Fill out your copyright notice in the Description page of Project Settings.


#include "DittoTransaction_Equip.h"

#include "BlueprintGameplayTagLibrary.h"
#include "DittoFragment_OutfitPart.h"
#include "DittoOutfits.h"
#include "Combee/Core/Container/CombeeContainer.h"
#include "Combee/Core/Item/CombeeItemInterface.h"
#include "CombeePresets/Common/Transactions/CombeeTransaction_Swap.h"

void UDittoTransaction_Equip::K2_OnExecute_Implementation()
{
	const auto PayloadPtr = Payload.GetPtr<FPayloadType>();

	if (!PayloadPtr)
	{
		MARK_TRANSACTION_FAILED_AND_RETURN();
	}

	const auto OutfitContainer = PayloadPtr->OutfitContainer;
	auto Part = PayloadPtr->Part;
	auto OutfitIndex = PayloadPtr->OutfitIndex;
	const auto OtherContainer = PayloadPtr->OtherContainer;
	const auto OtherIndex = PayloadPtr->OtherIndex;
	const auto bPreferEmpty = PayloadPtr->bPreferEmpty;

#pragma region Validation
	bool bOtherIndexValid;
	const auto OtherCellInfo = OtherContainer->GetCell(OtherIndex, bOtherIndexValid);

	if (!bOtherIndexValid || !OtherCellInfo.Item.GetObject())
	{
		MARK_TRANSACTION_FAILED_AND_RETURN();
	}

	const auto OutfitFragment = OtherCellInfo.Item->FindFragmentByClass<UDittoFragment_OutfitPart>();

	if (!OutfitFragment)
	{
		MARK_TRANSACTION_FAILED_AND_RETURN();
	}

	if (Part.IsValid())
	{
		check(UBlueprintGameplayTagLibrary::HasAllTags( OutfitFragment->Part,Part,true));
	}

	Part = OutfitFragment->Part;

	if (OutfitIndex == INDEX_NONE && Part.IsValid())
	{
		TArray<int32> OutIndices;
		int32 OutLast;
		bool OutFound;
		OutfitContainer->FindIndexByPart(Part, OutIndices, OutfitIndex, OutLast, OutFound);

		if (bPreferEmpty && !OutfitContainer->CheckCellEmpty(OutfitIndex))
		{
			for (int32 OutIndex : OutIndices)
			{
				if (OutfitContainer->CheckCellEmpty(OutIndex))
				{
					OutfitIndex = OutIndex;
					break;
				}
			}
		}
	}

	if (!OutfitContainer->CheckIndexIsValid(OutfitIndex))
	{
		MARK_TRANSACTION_FAILED_AND_RETURN();
	}
#pragma endregion


	const auto SwapPayload = FInstancedStruct::Make(FCombeeTransactionPayload_Common{
		.TargetContainer = OutfitContainer,
		.TargetIndex = OutfitIndex,
		.FromContainer = OtherContainer,
		.FromIndex = OtherIndex,
	});

	const auto SwapResult = ProcessTransaction<UCombeeTransaction_Swap>(SwapPayload);

	if (SwapResult->State != ECombeeExecutionState::Success)
	{
		MARK_TRANSACTION_FAILED_AND_RETURN();
	}
}
