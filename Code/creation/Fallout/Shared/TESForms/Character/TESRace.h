#pragma once

#include "Shared/FormComponents/TESForm.h"
#include "Shared/FormComponents/TESFullName.h"
#include "Shared/FormComponents/TESDescription.h"
#include "Shared/FormComponents/TESSpellList.h"
#include "Shared/FormComponents/BGSSkinForm.h"
#include "Shared/FormComponents/BGSBipedObjectForm.h"
#include "Shared/FormComponents/BGSKeywordForm.h"
#include "Shared/FormComponents/BGSAttackDataForm.h"
#include "Shared/FormComponents/BGSPropertySheet.h"
#include "Shared/FormComponents/BGSPreloadable.h"

namespace creation
{
    class TESRace : public TESForm, public TESFullName, public TESDescription, 
                    public TESSpellList, public BGSSkinForm, public BGSBipedObjectForm, 
                    public BGSKeywordForm, public BGSAttackDataForm, public BGSPropertySheet, 
                    public BGSPreloadable
    {
        uint8_t unkC0[0x6D8 - 0xC0];
    };

    static_assert(sizeof(TESRace) == 0x6D8);
}
