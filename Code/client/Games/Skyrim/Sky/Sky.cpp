
#include <Sky/Sky.h>
#include <TiltedOnlinePCH.h>

Sky* Sky::Get() noexcept
{
    POINTER_SKYRIMSE(Sky*, s_instance, 0x142F26B68 - 0x140000000);
    return *(s_instance.Get());
}

void Sky::Reset()
{
    using TReset = void(Sky*);
    POINTER_SKYRIMSE(TReset, ResetSky, 0x1403B4A20 - 0x140000000);

    ResetSky(this);
}

void Sky::SetSkyMode(SkyMode skyMode)
{
    using TSetState = void(Sky*, SkyMode);
    POINTER_SKYRIMSE(TSetState, SetSkyState, 0x1403B0F30 - 0x140000000);

    SetSkyState(this, skyMode);
}
