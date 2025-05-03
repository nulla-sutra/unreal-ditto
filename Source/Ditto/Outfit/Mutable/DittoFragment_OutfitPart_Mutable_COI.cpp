// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "DittoFragment_OutfitPart_Mutable_COI.h"
#include "DittoOutfitPartData_Mutable.h"
#include "MuCO/CustomizableSkeletalComponent.h"

#if WITH_EDITOR
#include "MuCOE/Nodes/CustomizableObjectNode.h"
#endif

UDittoFragment_OutfitPart_Mutable_COI::UDittoFragment_OutfitPart_Mutable_COI()
{
}

void UDittoFragment_OutfitPart_Mutable_COI::TakeOff_Implementation(const FInstancedStruct& PartData) const
{
    const auto Data = PartData.GetPtr<FDittoOutfitPartData_Mutable>();
    if (!Data) return;

    const auto Component = Data->CustomizableComponent;
    if (!ensure(Component))
    {
        return;
    }

    const auto Coi = Component->GetCustomizableObjectInstance();
    if (Coi && Coi->FindIntParameterNameIndex(Data->PartName) != INDEX_NONE)
    {
        Coi->SetDefaultValue(Data->PartName);
        Component->UpdateSkeletalMeshAsync();
        return;
    };

    return;
}

void UDittoFragment_OutfitPart_Mutable_COI::Wear_Implementation(const FInstancedStruct& PartData) const
{
    const auto Data = PartData.GetPtr<FDittoOutfitPartData_Mutable>();
    if (!Data || Data->PartName.IsEmpty())
    {
        return;;
    }

    const auto Component = Data->CustomizableComponent;
    if (!ensure(Component))
    {
        return;
    }

    const auto Coi = Component->GetCustomizableObjectInstance();
    const auto NameStr = Data->PartName;

    if (Coi && Coi->FindIntParameterNameIndex(NameStr) != INDEX_NONE)
    {
        Coi->SetIntParameterSelectedOption(NameStr, ObjectName);
        Coi->CopyParametersFromInstance(CustomizableObjectInstance);
        Component->UpdateSkeletalMeshAsync();
    }
}


#if WITH_EDITOR
void UDittoFragment_OutfitPart_Mutable_COI::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    const auto Property = PropertyChangedEvent.Property;

    if (Property)
    {
        if (Property->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, CustomizableObjectInstance))
        {
            if (CustomizableObjectInstance)
            {
                const auto CustomizableObject = CustomizableObjectInstance->GetCustomizableObject();
                const auto SourceProperty = UCustomizableObject::StaticClass()->FindPropertyByName("Source");
                UEdGraph* Graph = nullptr;
                SourceProperty->GetValue_InContainer(CustomizableObject, &Graph);

                if (!Graph)
                {
                    goto clear_object_name;
                }
                const auto RootNode = Cast<UCustomizableObjectNode>(Graph->Nodes[0]);
                if (!RootNode)
                {
                    goto clear_object_name;
                }

                const auto NameProperty = RootNode->GetClass()->FindPropertyByName("ObjectName");
                if (!NameProperty)
                {
                    goto clear_object_name;
                }

                NameProperty->GetValue_InContainer(RootNode, &ObjectName);
            }
            else
            {
            clear_object_name:
                ObjectName.Empty();
            }
        }
    }
}
#endif
