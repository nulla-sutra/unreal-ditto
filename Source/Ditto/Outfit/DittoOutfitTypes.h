// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DittoOutfitTypes.generated.h"

class UCombeeContainer;
class UDittoOutfits;

USTRUCT(BlueprintType)
struct DITTO_API FDittoOutfitTransactionPayload
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer Part;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UDittoOutfits> OutfitContainer;

	UPROPERTY(BlueprintReadWrite)
	int32 OutfitIndex = INDEX_NONE;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UCombeeContainer> OtherContainer;

	UPROPERTY(BlueprintReadWrite)
	int32 OtherIndex = INDEX_NONE;
};
