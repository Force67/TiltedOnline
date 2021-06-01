#pragma once

#include "BSExtraData.h"
#include "BSCore/BSTArray.h"

namespace creation
{
    class TESFaction;

    class ExtraFactionChanges : public BSExtraData
    {
    public:
        virtual ~ExtraFactionChanges();

        struct Entry
        {
            TESFaction* faction;
            int8_t rank;
        };

        uint8_t pad18[0x8];
        BSTArray<Entry> m_entries;
        void* unkPtr38;
    };

    static_assert(sizeof(ExtraFactionChanges::Entry) == 0x10);
    static_assert(sizeof(ExtraFactionChanges) == 0x40);
    static_assert(offsetof(ExtraFactionChanges, m_entries) == 0x20);
}
