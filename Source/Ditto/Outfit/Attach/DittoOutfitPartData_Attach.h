// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DittoOutfitPartData_Attach.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct DITTO_API FDittoOutfitPartData_Attach
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> MeshComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh;

	UPROPERTY(BlueprintReadWrite)
	FString PartName;
};
