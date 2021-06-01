#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class TESDescription : public BaseFormComponent
    {
        uint32_t unk8;
        uint32_t unkC;
        uint32_t unk10;
        uint32_t pad14;
    };

    static_assert(sizeof(TESDescription) == 0x18);
}
