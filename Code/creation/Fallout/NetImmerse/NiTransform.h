#pragma once

#include "NetImmerse/NiMatrix3.h"
#include "NetImmerse/NiPoint3.h"

namespace creation
{
    class NiTransform
    {
    public:
        NiMatrix3 m_Rotate;
        NiPoint3 m_Translate;
        float m_fScale;
    };

    static_assert(sizeof(NiTransform) == 0x40);
}
