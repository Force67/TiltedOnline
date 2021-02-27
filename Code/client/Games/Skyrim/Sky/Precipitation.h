#pragma once

#include <Sky/SkyObject.h>

class Precipitation : public SkyObject
{
public:
    virtual ~Precipitation() = default;

    char pad_0[128];
};

static_assert(sizeof(Precipitation) == 144);
