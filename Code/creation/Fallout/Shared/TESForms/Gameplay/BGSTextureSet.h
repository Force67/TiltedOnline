#pragma once

#include "Shared/TESForms/Objects/TESBoundObject.h"
#include "BSTextureSet.h"

namespace creation
{
    class BGSTextureSet : public TESBoundObject, public BSTextureSet
    {
        uint8_t pad78[0x350 - 0x78];
    };

    static_assert(sizeof(BGSTextureSet) == 0x350);
}
