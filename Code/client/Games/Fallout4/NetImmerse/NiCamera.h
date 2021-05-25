#pragma once

#include <NetImmerse/NiAVObject.h>

struct NiNode;

template<typename T>
struct NiRect
{
    T left;
    T right;
    T top;
    T bottom;
};

struct NiCamera : public NiAVObject
{
    virtual ~NiCamera() = default;
    
    static bool WorldPtToScreenPt3(float* matrix, const NiRect<float>* port, NiPoint3* p_in, float* x_out, float* y_out,
                            float* z_out, float zeroTolerance = 1e-5f);
};
