#pragma once

#include "BSCore/BSFixedString.h"
#include "BSScriptIFunction.h"

namespace creation
{
    namespace BSScript
    {
        class NativeFunction : public IFunction
        {
        public:
            void* pad8;
            BSFixedString functionName;
            BSFixedString typeName;
            uint8_t pad20[0x50 - 0x28];
            void* functionAddress;
        };

        static_assert(offsetof(NativeFunction, functionName) == 0x18);
        static_assert(offsetof(NativeFunction, typeName) == 0x20);
        static_assert(offsetof(NativeFunction, functionAddress) == 0x50);
        static_assert(sizeof(NativeFunction) == 0x58);
    }
}
