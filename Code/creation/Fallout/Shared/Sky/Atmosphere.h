#pragma once

#include "Shared/Sky/SkyObject.h"

namespace creation
{
    struct BSFogProperty;

    class Atmosphere final : public SkyObject
    {
    public:
        Atmosphere() = default;
        virtual ~Atmosphere() = default;

        void Update(Sky* apSky, float aTick) override;

    private:
        NiPointer<BSTriShape> m_pAtmosphereShape;
        NiPointer<BSFogProperty> m_pFogProp;
        NiPointer<NiNode> m_pUnkNode;
        NiPointer<BSTriShape> m_pUnkShape;
        bool m_bProcessFogDistanceStuff = true;
    };

    static_assert(sizeof(Atmosphere) == 0x38);
}
