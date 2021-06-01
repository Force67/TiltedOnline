#pragma once

#include "BaseFormComponent.h"
#include "BSCore/BSFixedString.h"

namespace creation
{
    class TESFullName : public BaseFormComponent
    {
    public:
        virtual ~TESFullName() = default;

    private:
        BSFixedString m_fullName;
    };

    static_assert(sizeof(TESFullName) == 0x10);
}
