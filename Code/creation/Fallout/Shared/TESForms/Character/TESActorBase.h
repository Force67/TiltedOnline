#pragma once

#include "Shared/TESForms/Objects/TESBoundAnimObject.h"
#include "Shared/FormComponents/TESActorBaseData.h"
#include "Shared/FormComponents/TESContainer.h"
#include "Shared/FormComponents/TESSpellList.h"
#include "Shared/FormComponents/TESAIForm.h"
#include "Shared/FormComponents/TESFullName.h"
#include "Shared/AI/ActorValueOwner.h"
#include "Shared/FormComponents/BGSDestructibleObjectForm.h"
#include "Shared/FormComponents/BGSSkinForm.h"
#include "Shared/FormComponents/BGSKeywordForm.h"
#include "Shared/FormComponents/BGSAttackDataForm.h"
#include "Shared/FormComponents/BGSPerkRankArray.h"
#include "Shared/FormComponents/BGSPropertySheet.h"

namespace creation
{
    class TESActorBase : public TESBoundAnimObject, public TESActorBaseData, public TESContainer,
                         public TESSpellList, public TESAIForm, public TESFullName, 
                         public ActorValueOwner, public BGSDestructibleObjectForm,
                         public BGSSkinForm, public BGSKeywordForm, public BGSAttackDataForm,
                         public BGSPerkRankArray, public BGSPropertySheet
    {
        virtual ~TESActorBase() override;
    };

    static_assert(sizeof(TESActorBase) == 0x1B0);
}
