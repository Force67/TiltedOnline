#pragma once

#include <NetImmerse/NiAVObject.h>
#include <NetImmerse/NiTObjectArray.h>
#include <NetImmerse/NiPointer.h>

struct NiNode : NiAVObject
{
    virtual ~NiNode() = default;

    // in reality:
    //NiTObjectArray<NiPointer<NiAVObject>> children;
    // but we are lazy:
    NiTObjectArray<NiAVObject*> children;
};

static_assert(sizeof(NiNode) == 0x128);
