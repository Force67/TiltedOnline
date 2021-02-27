#pragma once

#include <Sky/SkyObject.h>

class Sun : public SkyObject
{
public:
    virtual ~Sun() = default;

    char pad0[72];
};

static_assert(sizeof(Sun) == 88);
