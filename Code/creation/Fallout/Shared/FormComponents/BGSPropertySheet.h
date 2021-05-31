#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class BGSPropertySheet : public BaseFormComponent
    {
        virtual ~BGSPropertySheet();

        void* unk;
    };

    static_assert(sizeof(BGSPropertySheet) == 0x10);
}
