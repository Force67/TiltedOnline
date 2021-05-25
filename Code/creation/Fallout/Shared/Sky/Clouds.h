#pragma once

#include "NetImmerse/NiColor.h"
#include "Shared/Sky/SkyObject.h"

namespace creation
{
    class NiTexture;

    class Clouds final : public SkyObject
    {
    public:
        Clouds() = default;
        virtual ~Clouds() = default;

        void Update(Sky* apSky, float aTick) override;
    private:
        NiPointer<BSGeometry> m_pCloudGeometry[32];
        char pad32[8 * 32]{}; // This is some other array
        NiPointer<NiTexture> m_pTextures[32];
        NiColor m_ColorArray[32]{};
        float m_fAlphas[32]{ 0.f };
        uint16_t unk4 = 0;
        bool m_bUpdate = false;
    };

    static_assert(sizeof(Clouds) == 0x518);
}
