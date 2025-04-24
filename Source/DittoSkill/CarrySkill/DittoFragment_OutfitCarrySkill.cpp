// Copyright 2019-Present tarnishablec. All Rights Reserved.


#include "DittoFragment_OutfitCarrySkill.h"

#include "Combee/Core/Item/CombeeItemInterface.h"
#include "Combee/Subsystem/CombeeSubsystem.h"
#include "CombeePresets/Common/Fragments/CombeeFragment_Ownership.h"
#include "CombeePresets/Common/Transactions/CombeeTransaction_Append.h"
#include "CombeePresets/Common/Transactions/CombeeTransaction_Eject.h"
#include "CombeeTransaction/Subsystem/CombeeTransactionSubsystem.h"
#include "Ditto/Outfit/DittoOutfits.h"
#include "Dogars/Skill/DogarsItem_Skill.h"
#include "Dogars/Skill/DogarsSkillBag.h"

UDittoFragment_OutfitCarrySkill::UDittoFragment_OutfitCarrySkill()
{
    Rna = FInstancedStruct::Make<FFRnaDittoOutfitCarrySkill>();
}

void UDittoFragment_OutfitCarrySkill::OnInitialized_Implementation(const TScriptInterface<ICombeeItemInterface>& Item)
{
    const auto OwnerShip = Item->FindFragmentByClass<UCombeeFragment_Ownership>();
    if (!OwnerShip)
    {
        checkNoEntry();
        return;
    }

    const auto Subsystem = UCombeeSubsystem::Get(*this);
    ensure(Subsystem);
    const auto TransactionSubsystem = UCombeeTransactionSubsystem::Get(this);
    ensure(TransactionSubsystem);

    auto& RnaData = Rna.GetMutable<FFRnaDittoOutfitCarrySkill>();

    for (auto&& SkillClass : Skills)
    {
        if (SkillClass)
        {
            const auto Skill = Subsystem->Instantiate(SkillClass, {});
            RnaData.SkillInstances.Add(Skill);
        }
    }


    OwnerShip->PostTransfer.AddWeakLambda(
        this, [TransactionSubsystem,this](const FCombeeItemTransferContext& Context)
        {
            const auto FromContainer = Context.OldContainer;
            const auto ToContainer = Context.NewContainer;
            const auto SkillInstances = Rna.Get<FFRnaDittoOutfitCarrySkill>().SkillInstances;

            if (const auto OutfitsContainer = Cast<UDittoOutfits>(FromContainer))
            {
                const auto OwnerActor = OutfitsContainer->GetOwner();
                ensure(OwnerActor);

                const auto SkillBag = OwnerActor->GetComponentByClass<UDogarsSkillBag>();
                if (SkillBag)
                {
                    for (auto&& Skill : SkillInstances)
                    {
                        TArray<int32> OutIndices;
                        int32 First;
                        int32 Last;
                        bool bFound;
                        SkillBag->FindIndexByInstance(Skill, OutIndices, First, Last, bFound);

                        if (bFound)
                        {
                            ensure(First == Last && First != INDEX_NONE);
                            FCombeeTransactionPayload_Common EjectPayload{
                                .TargetContainer = SkillBag,
                                .TargetIndex = First
                            };
                            TransactionSubsystem->RequestTransaction(
                                OwnerActor, UCombeeTransaction_Eject::StaticClass(),
                                FInstancedStruct::Make(EjectPayload), {});
                        }
                    }
                }
            }

            if (const auto OutfitsContainer = Cast<UDittoOutfits>(ToContainer))
            {
                const auto OwnerActor = OutfitsContainer->GetOwner();
                ensure(OwnerActor);
                const auto SkillBag = OwnerActor->GetComponentByClass<UDogarsSkillBag>();
                if (SkillBag)
                {
                    for (auto&& Skill : SkillInstances)
                    {
                        FCombeeTransactionPayload_Append AppendPayload{
                            .bAutoExpand = true,
                            .Item = Skill,
                            .Amount = 1,
                            .TargetContainer = SkillBag
                        };
                        TransactionSubsystem->RequestTransaction(OwnerActor, UCombeeTransaction_Append::StaticClass(),
                                                                 FInstancedStruct::Make(AppendPayload), {});
                    }
                }
            }
        });
}
