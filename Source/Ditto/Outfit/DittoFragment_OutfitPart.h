// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Combee/Core/Fragment/CombeeFragment.h"
#include "CombeeUse/Usable/CombeeUsableFragment.h"
#include "UE5Coro.h"
#include "DittoFragment_OutfitPart.generated.h"

class UDittoOutfits;
class UCombeeContainer;
/**
 * 
 */
UCLASS(Abstract, DisplayName="Outfit Part (Ditto)")
class DITTO_API UDittoFragment_OutfitPart : public UCombeeFragment, public ICombeeUsableFragment
{
    GENERATED_BODY()

    friend class UDittoOutfits;

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTagContainer Part;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<TSubclassOf<UCombeeContainer>> Closets;

    virtual FVoidCoroutine OnUse(AController* const& Instigator, TInstancedStruct<FCombeeUseContext>& Payload,
                                 bool& bSuccess, const UE5Coro::TLatentContext<> _) override;

protected:
    UFUNCTION(BlueprintNativeEvent, Category="Ditto | Outfit Avatar", DisplayName="Take Off")
    void TakeOff(const FInstancedStruct& PartData) const;

    UFUNCTION(BlueprintNativeEvent, Category="Ditto | Outfit Avatar", DisplayName="Wear")
    void Wear(const FInstancedStruct& PartData) const;
};
