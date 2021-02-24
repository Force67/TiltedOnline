#pragma once

#include <NetImmerse/NiObject.h>

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

struct NiTexture : NiObject
{
    virtual ~NiTexture();

    
	// 28
    class RendererData
    {
      public:
        RendererData(uint32_t w, uint32_t h)
            : texture(nullptr), unk08(0), resourceView(nullptr), width(w), height(h), unk1C(1), unk1D(0x1C), unk1E(0),
              unk20(1), unk24(0x00130012)
        {
        }

        ID3D11Texture2D* texture;               // 00
        uint64_t unk08;                           // 08
        ID3D11ShaderResourceView* resourceView; // 10
        uint16_t width;                           // 18
        uint16_t height;                          // 1A
        uint8_t unk1C;                            // 1C
        uint8_t unk1D;                            // 1D
        uint16_t unk1E;                           // 1E
        uint32_t unk20;                           // 20
        uint32_t unk24;                           // 24
    };

    uint32_t unk10;                   // 10 - 6
    uint32_t unk14;                   // 14 - 3
    uint32_t unk18;                   // 18 - 2
    uint32_t unk1C;                   // 1C
    const char* name;               // 20
    uint32_t unk28;                   // 28 - FFFFFF
    uint32_t unk2C;                   // 2C
    NiTexture* prevTexture;         // 30
    NiTexture* nextTexture;         // 38
    void* resourceData; // 40
    RendererData* rendererData;     // 48
};
