#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class TESForm;

    class TESContainer : public BaseFormComponent
    {
    public:
        struct Entry
        {
            uint32_t count;
            TESForm* form;
            void* data;
        };

        Entry** entries;
        uint32_t count;
    };

    static_assert(sizeof(TESContainer) == 0x18);
}
