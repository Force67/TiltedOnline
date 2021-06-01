#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class BGSPreloadable : public BaseFormComponent
    {
    };

    static_assert(sizeof(BGSPreloadable) == 0x8);
}
