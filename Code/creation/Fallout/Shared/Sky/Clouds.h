#pragma once

#include <Sky/SkyObject.h>

class Clouds : public SkyObject
{
  public:
    virtual ~Clouds() = default;

    char pad_0[1312 - 16];
};

static_assert(sizeof(Clouds) == 1312);
