#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class BGSPropertySheet : public BaseFormComponent
    {
        void* unk;
    };

    static_assert(sizeof(BGSPropertySheet) == 0x10);
}
