// Copyright 2019-Present tarnishablec. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DittoOutfitAvatar.generated.h"

struct FCombeeCellMutationContext;
class UDittoOutfits;

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class DITTO_API UDittoOutfitAvatar : public UInterface
{
    GENERATED_BODY()
};

/**
 * 
 */
class DITTO_API IDittoOutfitAvatar
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintNativeEvent,
        Category="Outfit Avatar",
        BlueprintAuthorityOnly,
        DisplayName="Setup Outfit Avatar (Authority)",
        meta=(ForceAsFunction)
    )
    void SetupOutfitAvatar(UDittoOutfits* OutfitComponent);

    UFUNCTION(BlueprintNativeEvent,
        Category="Outfit Avatar",
        BlueprintAuthorityOnly,
        DisplayName="Receive Outfit Update (Authority)",
        meta=(ForceAsFunction))
    void ReceiveOutfitUpdate(UDittoOutfits* OutfitComponent, const FCombeeCellMutationContext& Context);
};
