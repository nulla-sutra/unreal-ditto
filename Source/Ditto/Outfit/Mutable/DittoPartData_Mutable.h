// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DittoPartData_Mutable.generated.h"

class UCustomizableSkeletalComponent;

USTRUCT(BlueprintType)
struct DITTO_API FDittoPartData_Mutable
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UCustomizableSkeletalComponent> CustomizableComponent;

    UPROPERTY(BlueprintReadWrite)
    FName PartName;    
};
