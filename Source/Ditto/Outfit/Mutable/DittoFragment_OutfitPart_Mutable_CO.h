// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ditto/Outfit/DittoFragment_OutfitPart.h"
#include "DittoFragment_OutfitPart_Mutable_CO.generated.h"

class UCustomizableObject;
/**
 * 
 */
UCLASS(DisplayName="OutfitPart Mutable Customizable Object (Ditto)")
class DITTO_API UDittoFragment_OutfitPart_Mutable_CO : public UDittoFragment_OutfitPart
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UCustomizableObject> CustomizableObject;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString ObjectName;

protected:
    virtual void TakeOff_Implementation(const FInstancedStruct& PartData) const override;
    virtual void Wear_Implementation(const FInstancedStruct& PartData) const override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
