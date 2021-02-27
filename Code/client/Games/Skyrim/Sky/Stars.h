#pragma once

#include <Sky/SkyObject.h>

class Stars : public SkyObject
{
public:
    virtual ~Stars() = default;

    char pad0[12];
};

static_assert(sizeof(Stars) == 32);
