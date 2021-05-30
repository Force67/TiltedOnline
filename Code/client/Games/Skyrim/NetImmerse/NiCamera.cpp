
#include <TiltedOnlinePCH.h>
#include <NetImmerse/NiCamera.h>

bool NiCamera::WorldPtToScreenPt3(const NiPoint3& in, NiPoint3& out, float tolerance)
{
    TP_THIS_FUNCTION(TW2S, bool, NiCamera, const NiPoint3*, float*, float*, float*, float);
    POINTER_SKYRIMSE(TW2S, s_w2s, 0x140C66530 - 0x140000000);

    return ThisCall(s_w2s, this, &in, &out.x, &out.y, &out.y, tolerance);
}
