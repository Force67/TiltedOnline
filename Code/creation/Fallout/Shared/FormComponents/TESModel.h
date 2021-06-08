#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class TESModel : public BaseFormComponent
    {
        uint8_t unk8[0x28];
    };

    static_assert(sizeof(TESModel) == 0x30);
}
