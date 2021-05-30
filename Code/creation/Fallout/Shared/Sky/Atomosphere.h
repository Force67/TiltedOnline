#pragma once

#include <Sky/SkyObject.h>

class Atmosphere : public SkyObject
{
  public:
    virtual ~Atmosphere() = default;

    char pad_0[40];
};

static_assert(sizeof(Atmosphere) == 56);
