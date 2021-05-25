#pragma once

#include <Games/Primitives.h>
#include <NetImmerse/NiPointer.h>

class Sky;
class NiNode;

class SkyObject
{
public:
    virtual ~SkyObject() = default;

    // Compiler on Fallout de-virtualized InitSkyObj
    virtual NiNode* GetRoot() = 0;
    virtual void Update(Sky* apSky, float afSecondsPassed) = 0;

private:
    NiPointer<NiNode> pObject;
};

static_assert(sizeof(SkyObject) == 16);
