#pragma once

#include "TESModel.h"

namespace creation
{
    class BGSModelMaterialSwap : public TESModel
    {
        uint8_t unk30[0x10];
    };

    static_assert(sizeof(BGSModelMaterialSwap) == 0x40);
}
