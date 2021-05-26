#pragma once

#include "Shared/Sky/SkyObject.h"

namespace creation
{
    class NiBillboardNode;
    class BSTriShape;
    class NiDirectionalLight;
    class BSShaderAccumulator;

    class Sun final : public SkyObject
    {
    public:
        Sun() = default;
        virtual ~Sun() = default;
        void Update(Sky* apSky, float aTick) override;

    private:
        NiPointer<NiBillboardNode> m_pBillboard1;
        NiPointer<NiBillboardNode> m_pBillboard2;
        NiPointer<BSTriShape> m_pBaseShape;
        NiPointer<BSTriShape> m_pUnkShape2;
        NiPointer<BSTriShape> m_pGlareShape;
        NiPointer<NiDirectionalLight> m_pLight;
        NiPointer<NiDirectionalLight> m_pCloudLight;
        float m_fGlare = 0.f;
        bool m_bunk3 = false;
        NiPointer<BSShaderAccumulator> m_pAccumulator;
    };

    static_assert(sizeof(Sun) == 0x58);
}
