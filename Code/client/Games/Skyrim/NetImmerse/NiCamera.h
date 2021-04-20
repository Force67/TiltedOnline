#pragma once

#include <NetImmerse/NiAVObject.h>

struct NiNode;

struct NiCamera : public NiAVObject
{
    virtual ~NiCamera() = default;
    
    bool WorldPtToScreenPt3(const NiPoint3& in, NiPoint3& out, float zeroTolerance = 1e-5f);

    NiNode* parent;
    NiAVObject* unk;
};
