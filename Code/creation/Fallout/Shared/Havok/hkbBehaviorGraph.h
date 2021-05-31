#pragma once

#include "Shared/Havok/hkbVariableValueSet.h"

class hkbStateMachine;

namespace creation
{
    class hkbBehaviorGraph
    {
    public:
        virtual ~hkbBehaviorGraph();

        uint8_t pad8[0xC0 - 0x8];
        hkbStateMachine* stateMachine;
        uint8_t padC8[0x110 - 0xC8];
        hkbVariableValueSet<uint32_t>* animationVariables; // 110
    };
}
