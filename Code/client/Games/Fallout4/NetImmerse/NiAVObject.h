#pragma once

#include <NetImmerse/NiObjectNET.h>

struct BSFixedString;

struct NiAVObject : NiObjectNET
{
    virtual ~NiAVObject() = default;

    virtual void sub_27() = 0;
    virtual void sub_28() = 0;
    virtual void sub_29() = 0;
    virtual void sub_30() = 0;
    virtual void sub_31() = 0;
    virtual void sub_32() = 0;
    virtual NiAVObject* GetObjectByName(const BSFixedString& aName) = 0;


};
