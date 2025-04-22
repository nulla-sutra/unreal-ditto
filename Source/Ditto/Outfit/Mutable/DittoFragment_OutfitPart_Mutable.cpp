// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "DittoFragment_OutfitPart_Mutable.h"

#include "DittoPartData_Mutable.h"
#include "MuCO/CustomizableSkeletalComponent.h"

void UDittoFragment_OutfitPart_Mutable::TakeOff_Implementation(const FInstancedStruct& PartData)
{
    const auto Data = PartData.GetPtr<FDittoPartData_Mutable>();
    if (!Data) return;

    const auto Component = Data->CustomizableComponent;
    if (!ensure(Component))
    {
        return;
    }

    Component->GetCustomizableObjectInstance()->SetDefaultValue(Data->PartName.ToString());
    Component->UpdateSkeletalMeshAsync();
}

void UDittoFragment_OutfitPart_Mutable::Wear_Implementation(const FInstancedStruct& PartData)
{
    const auto Data = PartData.GetPtr<FDittoPartData_Mutable>();
    if (!Data)
    {
        TakeOff(PartData);
        return;
    }

    const auto Component = Data->CustomizableComponent;
    if (!ensure(Component))
    {
        return;
    }

    Component->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(Data->PartName.ToString(), ObjectName);
    Component->UpdateSkeletalMeshAsync();
}

void UDittoFragment_OutfitPart_Mutable::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // TODO Auto Update Name Property
}
