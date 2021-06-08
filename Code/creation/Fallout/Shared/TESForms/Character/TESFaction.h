#pragma once

#include "Shared/FormComponents/TESForm.h"
#include "Shared/FormComponents/TESFullName.h"
#include "Shared/FormComponents/TESReactionForm.h"

namespace creation
{
    class TESFaction : public TESForm, public TESFullName, public TESReactionForm
    {
        uint8_t unk50[0xB0];
    };

    static_assert(sizeof(TESFaction) == 0x100);
}
