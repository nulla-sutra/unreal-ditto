// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "DittoMeshLibrary.h"

#include "SkeletalMergingLibrary.h"
#include "Rendering/SkeletalMeshLODRenderData.h"
#include "Rendering/SkeletalMeshRenderData.h"

USkeletalMesh* UDittoMeshLibrary::MergeSkeletonWithDummyMesh(TArray<USkeletalMesh*> Meshes)
{
    FSkeletonMergeParams MergeParams;

    const auto FirstSkeleton = (*Meshes.FindByPredicate([](USkeletalMesh* M)
    {
        return M != nullptr && M->GetSkeleton() != nullptr;
    }))->GetSkeleton();

    if (!FirstSkeleton)
    {
        return nullptr;
    }

    for (USkeletalMesh* Mesh : Meshes)
    {
        if (Mesh)
        {
            if (const auto Skeleton = Mesh->GetSkeleton())
            {
                if (Skeleton == FirstSkeleton || Skeleton->GetCompatibleSkeletons().Contains(FirstSkeleton))
                {
                    MergeParams.SkeletonsToMerge.AddUnique(Skeleton);
                }
            }
        }
    }

    if (MergeParams.SkeletonsToMerge.Num() == 0)
    {
        checkNoEntry();
        return nullptr;
    }

    const auto MergedSkeleton = USkeletalMergingLibrary::MergeSkeletons(MergeParams);

    const auto Dummy = NewObject<USkeletalMesh>();
    Dummy->AllocateResourceForRendering();
    Dummy->ReleaseResources();
    Dummy->ReleaseResourcesFence.Wait();

    Dummy->SetRefSkeleton(MergedSkeleton->GetReferenceSkeleton());
    Dummy->SetSkeleton(MergedSkeleton);

    const auto Resource = Dummy->GetResourceForRendering();
    auto& LODRenderData = *new FSkeletalMeshLODRenderData;
    Resource->LODRenderData.Add(&LODRenderData);

    for (auto&& Mesh : Meshes)
    {
        if (Mesh)
        {
            const auto& LodData = Mesh->GetResourceForRendering()->GetPendingFirstLOD(-1);
            if (LodData)
            {
                for (auto&& RequiredBoneIndex : LodData->RequiredBones)
                {
                    const auto BoneName = Mesh->GetRefSkeleton().GetBoneName(RequiredBoneIndex);
                    const auto NewIndex = MergedSkeleton->GetReferenceSkeleton().FindBoneIndex(BoneName);
                    if ((NewIndex >= 0))
                    {
                        LODRenderData.RequiredBones.AddUnique(NewIndex);
                    }
                }
            }
        }
    }

    Dummy->GetRefBasesInvMatrix().Empty();
    Dummy->CalculateInvRefMatrices();

    return Dummy;
}
