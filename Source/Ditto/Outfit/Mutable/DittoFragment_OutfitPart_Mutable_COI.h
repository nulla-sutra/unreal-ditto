// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ditto/Outfit/DittoFragment_OutfitPart.h"
#include "DittoFragment_OutfitPart_Mutable_COI.generated.h"

class UCustomizableObjectInstance;
/**
 * 
 */
UCLASS(DisplayName="OutfitPart Mutable Customizable Object Instance (Ditto)")
class DITTO_API UDittoFragment_OutfitPart_Mutable_COI : public UDittoFragment_OutfitPart
{
    GENERATED_BODY()
    UDittoFragment_OutfitPart_Mutable_COI();

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UCustomizableObjectInstance> CustomizableObjectInstance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FString ObjectName;

protected:
    virtual bool K2_TakeOff_Implementation(const FInstancedStruct& PartData) override;
    virtual bool K2_Wear_Implementation(const FInstancedStruct& PartData) override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
