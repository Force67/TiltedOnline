#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class TESAIForm : public BaseFormComponent
    {
        uint8_t	unk8[0x28 - 0x8];
    };

    static_assert(sizeof(TESAIForm) == 0x28);
}
