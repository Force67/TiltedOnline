#pragma once

#include <Sky/SkyObject.h>
#include <Misc/BSString.h>

class Moon : public SkyObject
{
public:
    virtual ~Moon() = default;

    char pad_0[48];
    BSString texNameFull;
    BSString texNameWan;
    BSString texNameHalfWan;
    BSString texNameOneWan;
    BSString texNameNew;
    BSString texNameOneWax;
    BSString texNameHalfWax;
    BSString texNameThreeWax;
    char pad_128[48];
    float fSecundaAngleFadeStart;
    float fSecundaAngleFadeEnd;
    float fSecundaAngleShadowEarlyFade;
    float fSecundaSpeed;
    float fSecundaZOffset;
    int iSecundaSize;
    char pad_200[16];
};

static_assert(sizeof(Moon) == 216);
