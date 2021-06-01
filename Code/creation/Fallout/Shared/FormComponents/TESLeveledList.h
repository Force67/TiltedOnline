#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class TESForm;

    class TESLeveledList : public BaseFormComponent
    {
    public:
        struct Entry
        {
            TESForm* form;
            void* unkPtr;
            uint16_t minLevel;
            uint16_t maxLevel;
            uint8_t unk14; // Specific to fallout 4
        };

        struct Content
        {
            uint64_t length; // it's set to the size of the allocated array for some reason
            Entry entries[0];
        };

        uint8_t pad8[0x18 - 0x8];
        Content* m_pContent;
        uint8_t pad20[0x28 - 0x20];
        bool m_bHasData;
        uint8_t m_uiCount;
    };

    static_assert(sizeof(TESLeveledList::Entry) == 0x18);
    static_assert(offsetof(TESLeveledList::Entry, unk14) == 0x14);
    static_assert(offsetof(TESLeveledList, m_pContent) == 0x18);
    static_assert(offsetof(TESLeveledList, m_bHasData) == 0x28);
    static_assert(offsetof(TESLeveledList, m_uiCount) == 0x29);
    static_assert(sizeof(TESLeveledList) == 0x30);
}
