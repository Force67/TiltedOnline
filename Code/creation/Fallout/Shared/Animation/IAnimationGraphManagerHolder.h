#pragma once

#include <stdafx.h>

class BSAnimationGraphManager;
class BSFixedString;
template<class T> class BSTSmartPointer;

namespace creation
{
    class IAnimationGraphManagerHolder
    {
    public:
        virtual ~IAnimationGraphManagerHolder();

        virtual bool SendAnimationEvent(BSFixedString* apAnimEvent);

        virtual void sub_2a();
        virtual void sub_2c();

        virtual bool GetBSAnimationGraph(BSTSmartPointer<BSAnimationGraphManager>* aPtr) const;
        virtual uint32_t sub_3();
        virtual uint32_t sub_4();
        virtual uint32_t sub_5();
        virtual uint32_t sub_6();
        virtual uint32_t sub_7();
        virtual uint32_t sub_8();
        virtual uint32_t sub_9();
        virtual uint32_t sub_A();
        virtual uint32_t sub_B();
        virtual uint32_t sub_C();
        virtual uint32_t sub_D();
        virtual uint32_t sub_E();
        virtual uint32_t sub_F();

        virtual uint32_t sub_12();

        virtual bool GetVariableFloat(BSFixedString* apVariable, float* apReturn);
        virtual bool GetVariableInt(BSFixedString* apVariable, uint32_t* apReturn);
        virtual bool GetVariableBool(BSFixedString* apVariable, bool* apReturn);

        bool SetVariableFloat(BSFixedString* apVariable, float aValue);
        bool IsReady();

        bool ReSendAnimationEvent(BSFixedString* apAnimEvent);
    };
}
