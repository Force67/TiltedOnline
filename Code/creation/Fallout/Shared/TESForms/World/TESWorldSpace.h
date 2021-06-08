#pragma once

#include "Shared/FormComponents/TESForm.h"
#include "Shared/FormComponents/TESFullName.h"
#include "Shared/FormComponents/TESTexture.h"

namespace creation
{
    class TESWorldSpace : public TESForm, public TESFullName, public TESTexture
    {
        uint8_t unk40[0x2D0 - 0x40];
    };

    static_assert(sizeof(TESWorldSpace) == 0x2D0);
}

