#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class TESForm;

    class BGSPerkRankArray : public BaseFormComponent
    {
        virtual ~BGSPerkRankArray();

        TESForm** m_pPerks; // 8
        uint32_t m_uiCount; // 10
        uint32_t pad;
    };

    static_assert(sizeof(BGSPerkRankArray) == 0x18);
}
