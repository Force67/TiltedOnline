#pragma once

namespace creation
{
    class hkbStateMachine
    {
    public:
        virtual ~hkbStateMachine();

        uint8_t pad8[0x38 - 0x8];
        char* name;
    };

    static_assert(offsetof(hkbStateMachine, name) == 0x38);
}
