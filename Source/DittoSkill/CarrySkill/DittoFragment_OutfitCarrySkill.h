// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Combee/Core/Fragment/CombeeFragment.h"
#include "DittoFragment_OutfitCarrySkill.generated.h"

class UDogarsItem_Skill;

USTRUCT(BlueprintType)
struct DITTOSKILL_API FFRnaDittoOutfitCarrySkill
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    TArray<TObjectPtr<UDogarsItem_Skill>> SkillInstances;
};

/**
 * 
 */
UCLASS(DisplayName = "Outfit Carry Skill (Ditto Skill)")
class DITTOSKILL_API UDittoFragment_OutfitCarrySkill : public UCombeeFragment
{
    GENERATED_BODY()

    UDittoFragment_OutfitCarrySkill();

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly,
        meta=(AllowAbstract = false))
    TArray<TSubclassOf<UDogarsItem_Skill>> Skills;

    virtual void OnInitialized_Implementation(const TScriptInterface<ICombeeItemInterface>& Item) override;
};
