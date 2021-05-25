#pragma once

#include <Games/Primitives.h>

struct NiRTTI;
struct NiTriBasedGeom;

struct NiObject : NiRefObject
{
    virtual ~NiObject() = 0;

    virtual NiRTTI* GetRTTI() = 0;

    virtual void sub_3() = 0;
    virtual void sub_4() = 0;
    virtual void sub_5() = 0;
    virtual void sub_6() = 0;
    virtual void sub_7() = 0;
    virtual void sub_8() = 0;
    virtual void sub_9() = 0;
    virtual void sub_A() = 0;
    virtual void sub_B() = 0;
    virtual void sub_C() = 0;
    virtual void sub_D() = 0;
    virtual void sub_E() = 0;
    virtual void sub_F() = 0;
    virtual void sub_10() = 0;
    virtual void sub_11() = 0;
    virtual void sub_12() = 0;
    virtual void sub_13() = 0;
    virtual void sub_14() = 0;
    virtual void sub_15() = 0;
    virtual void sub_16() = 0;
    virtual void sub_17() = 0;
    virtual void sub_18() = 0;
    virtual void sub_19() = 0;
    virtual void sub_1A() = 0;
    virtual void sub_1B() = 0;
    virtual void sub_1C() = 0;
    virtual void sub_1D() = 0;
    virtual void sub_1E() = 0;
    virtual void sub_1F() = 0;
    virtual void sub_20() = 0;
    virtual void sub_21() = 0;
    virtual void sub_22() = 0;
    virtual void sub_23() = 0;
    virtual void sub_24() = 0;
    virtual void sub_25() = 0;
    virtual void sub_26() = 0;
};
