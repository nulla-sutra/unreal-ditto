// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DittoOutfitTypes.h"
#include "CombeeTransaction/Transaction/CombeeTransaction.h"
#include "DittoTransaction_Equip.generated.h"

USTRUCT(BlueprintType)
struct DITTO_API FDittoOutfitTransactionPayload_Equip : public FDittoOutfitTransactionPayload
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    bool bPreferEmpty = true;
};

/**
 * 
 */
UCLASS(DisplayName="Outfit Equip (Ditto)")
class DITTO_API UDittoTransaction_Equip : public UCombeeTransaction
{
    GENERATED_BODY()

    using FPayloadType = FDittoOutfitTransactionPayload_Equip;

    virtual void OnExecute_Implementation() override;
};
