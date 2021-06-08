#pragma once

#include "Shared/FormComponents/TESForm.h"

namespace creation
{
    class TESCombatStyle : public TESForm
    {
        uint8_t unk20[0xC8];
    };

    static_assert(sizeof(TESCombatStyle) == 0xE8);
}
