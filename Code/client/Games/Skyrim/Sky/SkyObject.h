#pragma once

#include <Games/Primitives.h>

class SkyObject
{
public:
    virtual ~SkyObject() = default;

    virtual void* GetSomething() = 0;
    virtual void* CreateSomething(int param) = 0;
    virtual void m3(void* someObject, int param) = 0;

    __int64 object;
};
