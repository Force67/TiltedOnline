#pragma once

#include "BGSKeyword.h"

namespace creation
{
    class BGSAction : public BGSKeyword
    {
        uint32_t m_uiCounter;
        uint8_t pad2C[0x4];
    };

    static_assert(sizeof(BGSAction) == 0x30);
}
