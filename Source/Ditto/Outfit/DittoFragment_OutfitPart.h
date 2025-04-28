// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Combee/Core/Fragment/CombeeFragment.h"
#include "CombeeUse/Usable/CombeeUsableFragment.h"
#include "UE5Coro.h"
#include "DittoFragment_OutfitPart.generated.h"

struct FCombeeCellMutationContext;
class UDittoOutfits;
class UCombeeContainer;
/**
 * 
 */
UCLASS(Abstract, DisplayName="Outfit Part (Ditto)")
class DITTO_API UDittoFragment_OutfitPart : public UCombeeFragment, public ICombeeUsableFragment
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTagContainer Part;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<TSubclassOf<UCombeeContainer>> Closets;

    virtual FVoidCoroutine OnUse(AController* const& Instigator, TInstancedStruct<FCombeeUseContext>& Payload,
                                 bool& bSuccess, const UE5Coro::TLatentContext<> _) override;

// private:
    UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="Ditto | Outfit Avatar")
    void TakeOff(const FInstancedStruct& PartData);

    UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="Ditto | Outfit Avatar")
    void Wear(const FInstancedStruct& PartData);

protected:
    UFUNCTION(BlueprintNativeEvent, Category="Ditto | Outfit Avatar", DisplayName="TakeOff")
    bool K2_TakeOff(const FInstancedStruct& PartData);

    UFUNCTION(BlueprintNativeEvent, Category="Ditto | Outfit Avatar", DisplayName="Wear")
    bool K2_Wear(const FInstancedStruct& PartData);

public:
    UFUNCTION(BlueprintCallable, Category="Ditto | Outfit", BlueprintAuthorityOnly)
    static void ProcessOutfitsMutation(UDittoOutfits* OutfitsComponent, const FCombeeCellMutationContext& Context);
};
