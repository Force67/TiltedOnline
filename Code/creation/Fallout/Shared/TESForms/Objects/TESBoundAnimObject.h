#pragma once

#include "TESBoundObject.h"

namespace creation
{
    class TESBoundAnimObject : public TESBoundObject
    {
    };

    static_assert(sizeof(TESBoundAnimObject) == 0x68);
}
