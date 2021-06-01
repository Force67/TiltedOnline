#pragma once

#include "TESForm.h"

namespace creation
{
    class BGSStoryManagerTreeForm : public TESForm
    {
        uint64_t unk20;
    };

    static_assert(sizeof(BGSStoryManagerTreeForm) == 0x28);
}
