#pragma once

#include "Shared/FormComponents/TESForm.h"

namespace creation
{
    class BGSVoiceType : public TESForm
    {
        uint8_t pad20[0x18];
    };

    static_assert(sizeof(BGSVoiceType) == 0x38);
}
