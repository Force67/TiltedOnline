
#include <TiltedOnlinePCH.h>
#include <NetImmerse/NiCamera.h>

using TWorldPtToScreenPt3 = bool(float*, const NiRect<float>*, NiPoint3*, float*, float*, float*, float);
static TWorldPtToScreenPt3* s_WorldPtToScreenPt3;

bool NiCamera::WorldPtToScreenPt3(float* matrix, const NiRect<float>* port, NiPoint3* p_in, float* x_out, float* y_out,
                                  float* z_out, float zeroTolerance)
{
    return s_WorldPtToScreenPt3(matrix, port, p_in, x_out, y_out, z_out, zeroTolerance);
}

static TiltedPhoques::Initializer s_Init([]() 
{
    POINTER_FALLOUT4(TWorldPtToScreenPt3, s_w2s, 0x141BADB00 - 0x140000000);
    s_WorldPtToScreenPt3 = s_w2s.Get();
});
