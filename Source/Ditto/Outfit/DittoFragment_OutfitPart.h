// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Combee/Core/Fragment/CombeeFragment.h"
#include "CombeeUse/Usable/CombeeUsableFragment.h"
#include "UE5Coro.h"
#include "DittoFragment_OutfitPart.generated.h"

class UCombeeContainer;
/**
 * 
 */
UCLASS(Abstract)
class DITTO_API UDittoFragment_OutfitPart : public UCombeeFragment, public ICombeeUsableFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Outfit")
	FGameplayTagContainer Part;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Outfit")
	TArray<TSubclassOf<UCombeeContainer>> Closets;

	virtual FVoidCoroutine OnUse(AController* const& Instigator, TInstancedStruct<FCombeeUsePayload>& Payload,
	                             bool& bSuccess, const UE5Coro::TLatentContext<> _) override;

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="Outfit Avatar")
	void TakeOff(const FInstancedStruct& PartData);

	UFUNCTION(BlueprintNativeEvent, Category="Outfit Avatar", DisplayName="TakeOff")
	void K2_TakeOff(const FInstancedStruct& PartData);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category="Outfit Avatar")
	void Wear(const FInstancedStruct& PartData);

	UFUNCTION(BlueprintNativeEvent, Category="Outfit Avatar", DisplayName="Wear")
	void K2_Wear(const FInstancedStruct& PartData);
};
