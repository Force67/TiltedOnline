#pragma once

#include <Sky/SkyObject.h>

class BSGeometry;

class Clouds : public SkyObject
{
public:
    virtual ~Clouds() = default;

    NiPointer<BSGeometry> pClouds[32];
    char pad[0x500 - 8 * 32];
    uint16_t usLayerCount;
    bool bForceUpdate;
};

static_assert(offsetof(Clouds, Clouds::pClouds) == 0x10);
