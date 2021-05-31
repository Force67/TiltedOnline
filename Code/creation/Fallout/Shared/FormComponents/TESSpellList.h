#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class TESForm;
    class TESSpellList : BaseFormComponent
    {
      public:
        struct SpellData
        {
            void** spells;
            TESForm** unk4;
            void** shouts;
            uint32_t spellCount;
            uint32_t unk4Count;
            uint32_t shoultCount;
        };

        SpellData* m_pSpellData;

        void Initialize();
    };

    static_assert(sizeof(TESSpellList) == 0x10);
}
