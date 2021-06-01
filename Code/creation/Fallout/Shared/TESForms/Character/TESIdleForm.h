#pragma once

#include "Shared/FormComponents/TESForm.h"

namespace creation
{
    class TESIdleForm : public TESForm
    {
        uint8_t unk20[0x50];
    };

    static_assert(sizeof(TESIdleForm) == 0x70);
}
