#pragma once

#include "BaseFormComponent.h"
#include "NetImmerse/NiPointer.h"

namespace creation
{
    class BGSAttackDataMap : public NiRefObject
    {
        virtual ~BGSAttackDataMap();
    };

    static_assert(sizeof(BGSAttackDataMap) == 0x10);

    class BGSAttackDataForm : public BaseFormComponent
    {
        virtual ~BGSAttackDataForm();

        NiPointer<BGSAttackDataMap> attackDataMap;
    };

    static_assert(sizeof(BGSAttackDataForm) == 0x10);
}
