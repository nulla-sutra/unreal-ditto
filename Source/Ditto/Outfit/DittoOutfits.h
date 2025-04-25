// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DittoOutfitLayout.h"
#include "Combee/Core/Container/CombeeContainer.h"

#include "DittoOutfits.generated.h"


UCLASS(ClassGroup=(Outfit), meta=(BlueprintSpawnableComponent))
class DITTO_API UDittoOutfits : public UCombeeContainer
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UDittoOutfits();
    virtual bool CheckItemCompatible_Implementation(const TScriptInterface<ICombeeItemInterface>& Item,
                                                    const int32& Index) const override;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
    UPROPERTY(ReplicatedUsing=OnRep_Avatar, BlueprintReadWrite, Category="Outfit", Setter)
    TObjectPtr<AActor> Avatar;

    UPROPERTY(ReplicatedUsing=OnReq_Layout, BlueprintReadOnly, Category="Outfit")
    FDittoOutfitLayout Layout = FDittoOutfitLayout(this);

    UFUNCTION(BlueprintAuthorityOnly)
    void SetAvatar(AActor* InAvatar);

    UFUNCTION(BlueprintNativeEvent)
    void OnRep_Avatar();

    UFUNCTION(BlueprintNativeEvent)
    void OnReq_Layout();

    UFUNCTION(BlueprintCallable, Category="Outfit", meta=(AutoCreateRefTerm="PartData"), BlueprintAuthorityOnly)
    FDittoOutfitLayoutPart RegisterPart(FGameplayTagContainer Part,
                                        const FInstancedStruct& PartData = FInstancedStruct());

    UFUNCTION(BlueprintCallable, Category="Outfit", BlueprintAuthorityOnly)
    void ResetLayout();

    UFUNCTION(BlueprintPure, Category="Outfit")
    FDittoOutfitLayoutPart RetrievePartInfo(int32 Index) const;

    UFUNCTION(BlueprintPure, Category="Outfit", meta=(AutoCreateRefTerm="Part"))
    void FindIndexByPart(const FGameplayTagContainer& Part,
                         TArray<int32>& OutIndices,
                         int32& First,
                         int32& Last,
                         bool& Found) const;

    UFUNCTION(BlueprintPure, Category="Outfit")
    static bool CheckPartInfoValid(const FDittoOutfitLayoutPart& Info)
    {
        return Info.IsValid();
    }

protected:
    UFUNCTION()
    void HandleAuthorityCellMutation(const FCombeeCellMutationContext& Context);
};
