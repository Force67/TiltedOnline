#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class TESReactionForm : public BaseFormComponent
    {
        uint8_t unk8[0x18];
    };

    static_assert(sizeof(TESReactionForm) == 0x20);
}
