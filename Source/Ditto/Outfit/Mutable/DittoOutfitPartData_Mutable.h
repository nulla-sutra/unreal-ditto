// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DittoOutfitPartData_Mutable.generated.h"

class UCustomizableSkeletalComponent;

USTRUCT(BlueprintType)
struct DITTO_API FDittoOutfitPartData_Mutable
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UCustomizableSkeletalComponent> CustomizableComponent;

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<USkeletalMeshComponent> MeshComponent;

    UPROPERTY(BlueprintReadWrite)
    FString PartName;

    UPROPERTY(BlueprintReadWrite)
    TSubclassOf<UAnimInstance> DefaultLink;
};
