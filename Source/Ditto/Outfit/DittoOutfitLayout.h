// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "DittoOutfitLayout.generated.h"

class UDittoOutfits;

USTRUCT(BlueprintType)
struct DITTO_API FDittoOutfitLayoutPart
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FGameplayTagContainer Part;

	UPROPERTY(BlueprintReadOnly)
	int32 ContainerIndex = INDEX_NONE;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FInstancedStruct PartData;

	bool IsValid() const
	{
		return Part.IsValid() && ContainerIndex != INDEX_NONE;
	}
};


USTRUCT(BlueprintType)
struct DITTO_API FDittoOutfitLayout
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UDittoOutfits> Owner;

	UPROPERTY()
	TArray<TInstancedStruct<FDittoOutfitLayoutPart>> PartsRegistry;

	int32 Version = 0;

	FDittoOutfitLayout()
	{
	}

	explicit FDittoOutfitLayout(UDittoOutfits* Outfit): Owner(Outfit)
	{
	};


	FDittoOutfitLayoutPart Register(const FGameplayTagContainer& Part, const FInstancedStruct& PartData)
	{
		const FDittoOutfitLayoutPart Entry{
			.Part = Part,
			.ContainerIndex = PartsRegistry.Num(),
			.PartData = PartData
		};
		const auto& Info = TInstancedStruct<FDittoOutfitLayoutPart>::Make(Entry);
		PartsRegistry.Add(Info);
		Version++;
		return Entry;
	}

	void Reset()
	{
		PartsRegistry.Reset();
		Version++;
	}
};
