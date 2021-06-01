#pragma once

#include "Shared/FormComponents/TESForm.h"
#include "BSCore/BSFixedString.h"

namespace creation
{
    class BGSKeyword : public TESForm
    {
        BSFixedString m_keyword;
    };

    static_assert(sizeof(BGSKeyword) == 0x28);
}

