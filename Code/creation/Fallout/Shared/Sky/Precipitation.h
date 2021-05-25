#pragma once

#include "BSCore/BSFixedString.h"
#include "Shared/Sky/SkyObject.h"

namespace creation
{
    class Precipitation final : public SkyObject
    {
    public:
        Precipitation() = default;
        virtual ~Precipitation() = default;

        void Update(Sky* apSky, float aTick) override;

    private:
        char pad0[0x60];
        BSFixedString m_waterName;
        char padx[0x8];
        NiPointer<BSGeometry> m_pGeometry1;
        NiPointer<BSGeometry> m_pGemetry2;
        float m_fSize;
        float m_fdensity1;
        float m_funk3;
        float m_fWetness;
    };

    static_assert(sizeof(Precipitation) == 0xA0);
}
