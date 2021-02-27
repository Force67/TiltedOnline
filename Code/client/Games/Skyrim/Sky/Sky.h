#pragma once

class Atomosphere;
class Stars;
class Sun;
class Clouds;
class Moon;
class Precipitation;

class Sky
{
public:
    virtual ~Sky() = default;

    static Sky* Get() noexcept;

    char pad_0[112];
    Atomosphere* atomsphere;
    Stars* stars;
    Sun* sun;
    Clouds* clouds;
    Moon* masser;
    Moon* secunda;
    Precipitation* precipitation;
    char pad_160[300];
    float fSecondsBetweenWindowUpdates;
    char pad_468[128];
    float fAuroraFadeInStart;
    float fAuroraFadeIn;
    float fAuroraFadeOutStart;
    float fAuroraFadeOut;
    char pad_612[88];

    void Reset();
};

static_assert(sizeof(Sky) == 712);
