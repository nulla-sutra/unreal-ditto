// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DittoMeshLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DITTO_API UDittoMeshLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

    UFUNCTION(BlueprintCallable, Category="Ditto | Mesh")
    static USkeletalMesh* MergeSkeletonWithDummyMesh(TArray<USkeletalMesh*> Meshes);
};
