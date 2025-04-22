// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ditto/Outfit/DittoFragment_OutfitPart.h"
#include "DittoFragment_OutfitPart_Mutable.generated.h"

class UCustomizableObject;
/**
 * 
 */
UCLASS(DisplayName="OutfitPart Mutable (Ditto)")
class DITTO_API UDittoFragment_OutfitPart_Mutable : public UDittoFragment_OutfitPart
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UCustomizableObject> CustomizableObject;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString ObjectName;

protected:
    virtual void TakeOff_Implementation(const FInstancedStruct& PartData) override;
    virtual void Wear_Implementation(const FInstancedStruct& PartData) override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
