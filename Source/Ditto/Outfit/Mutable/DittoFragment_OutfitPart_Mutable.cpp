// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "DittoFragment_OutfitPart_Mutable.h"

#include "DittoPartData_Mutable.h"
#include "MuCO/CustomizableObject.h"
#include "MuCO/CustomizableSkeletalComponent.h"

#if WITH_EDITOR
#include "MuCOE/Nodes/CustomizableObjectNode.h"
#endif


void UDittoFragment_OutfitPart_Mutable::TakeOff_Implementation(const FInstancedStruct& PartData)
{
    const auto Data = PartData.GetPtr<FDittoPartData_Mutable>();
    if (!Data) return;

    const auto Component = Data->CustomizableComponent;
    if (!ensure(Component))
    {
        return;
    }

    const auto Coi = Component->GetCustomizableObjectInstance();
    if (Coi->FindIntParameterNameIndex(Data->PartName.ToString()) != INDEX_NONE)
    {
        Component->GetCustomizableObjectInstance()->SetDefaultValue(Data->PartName.ToString());
        Component->UpdateSkeletalMeshAsync();
    };
}

void UDittoFragment_OutfitPart_Mutable::Wear_Implementation(const FInstancedStruct& PartData)
{
    const auto Data = PartData.GetPtr<FDittoPartData_Mutable>();
    if (!Data || !Data->PartName.IsValid())
    {
        TakeOff(PartData);
        return;
    }

    const auto Component = Data->CustomizableComponent;
    if (!ensure(Component))
    {
        return;
    }

    const auto Coi = Component->GetCustomizableObjectInstance();
    
    if (Coi->FindIntParameterNameIndex(Data->PartName.ToString()) != INDEX_NONE)
    {
        Component->GetCustomizableObjectInstance()->SetIntParameterSelectedOption(Data->PartName.ToString(), ObjectName);
        Component->UpdateSkeletalMeshAsync();
    }
}

#if WITH_EDITOR
void UDittoFragment_OutfitPart_Mutable::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    const auto Property = PropertyChangedEvent.Property;

    if (Property)
    {
        if (Property->GetFName() == GET_MEMBER_NAME_CHECKED(ThisClass, CustomizableObject))
        {
            if (CustomizableObject)
            {
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
