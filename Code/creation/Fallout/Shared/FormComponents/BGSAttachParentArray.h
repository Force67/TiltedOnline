#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class BGSAttachParentArray : public BaseFormComponent
    {
        virtual ~BGSAttachParentArray();

        void* arr;
        uint32_t count;
        uint32_t pad;
    };
}
