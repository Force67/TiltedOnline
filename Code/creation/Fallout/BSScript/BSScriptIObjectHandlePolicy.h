#pragma once

#include "Shared/FormComponents/TESForm.h"

namespace creation
{
    class TESObjectREFR;

    namespace BSScript
    {
        class IObjectHandlePolicy
        {
            virtual ~IObjectHandlePolicy() = 0;

            virtual void sub_01();
            virtual void sub_02();
            virtual void sub_03();
            virtual void sub_04();
            virtual void sub_05();
            virtual void sub_06();
            virtual uint64_t GetHandle(FormType aFormType, TESObjectREFR* apObject);
        };
    }
}
