#pragma once

namespace creation
{
    template<class T>
    class hkbVariableValueSet
    {
    public:
        virtual ~hkbVariableValueSet();

        uint8_t pad8[0x8]; // 8
        T* data; // 10
        uint32_t size; // 18
    };
}
