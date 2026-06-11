// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ditto/Outfit/DittoFragment_OutfitPart.h"
#include "DittoFragment_OutfitPart_Attach.generated.h"

/**
 * 
 */
UCLASS(DisplayName="OutfitPart Attach (Ditto)")
class DITTO_API UDittoFragment_OutfitPart_Attach : public UDittoFragment_OutfitPart
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> AnimOverride;

	virtual void TakeOff_Implementation(const FInstancedStruct& PartData) const override;
	virtual void Wear_Implementation(const FInstancedStruct& PartData) const override;
};
