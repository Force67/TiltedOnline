#pragma once

#include "NetImmerse/NiPoint3.h"

namespace creation
{
    class NiBound
    {
    public:
        NiBound() = default;

    protected:
        NiPoint3 m_kCenter;
        float m_fRadius;

    private:
        static float ms_fFuzzFactor;
        static float ms_fTolerance;
    };

    static_assert(sizeof(NiBound) == 0x10);
}
