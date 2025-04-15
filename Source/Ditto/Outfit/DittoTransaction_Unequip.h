// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DittoOutfitTypes.h"
#include "Combee/Framework/Transaction/CombeeTransaction.h"
#include "DittoTransaction_Unequip.generated.h"

USTRUCT(BlueprintType)
struct DITTO_API FDittoOutfitTransactionPayload_Unequip : public FDittoOutfitTransactionPayload
{
	GENERATED_BODY()
};


/**
 * 
 */
UCLASS()
class DITTO_API UDittoTransaction_Unequip : public UCombeeTransaction
{
	GENERATED_BODY()

	using FPayloadType = FDittoOutfitTransactionPayload_Unequip;

	virtual void K2_OnExecute_Implementation() override;
};
