// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "DittoFragment_OutfitPart_Attach.h"

#include "DittoOutfitPartData_Attach.h"

void UDittoFragment_OutfitPart_Attach::TakeOff_Implementation(const FInstancedStruct& PartData) const
{
	const auto* Data = PartData.GetPtr<FDittoOutfitPartData_Attach>();
	if (!Data) return;

	const auto Comp = Data->MeshComponent;

	if (!Comp) return;

	const auto DefaultMesh = Data->DefaultMesh;

	Comp->SetSkeletalMesh(DefaultMesh);
}

void UDittoFragment_OutfitPart_Attach::Wear_Implementation(const FInstancedStruct& PartData) const
{
	const auto Data = PartData.GetPtr<FDittoOutfitPartData_Attach>();
	if (!Data) return;

	const auto Comp = Data->MeshComponent;
	if (!Comp) return;

	if (ensure(Mesh))
	{
		Comp->SetSkeletalMesh(Mesh);
		if (AnimOverride)
		{
			Comp->SetAnimInstanceClass(AnimOverride);
		}
	}
}
