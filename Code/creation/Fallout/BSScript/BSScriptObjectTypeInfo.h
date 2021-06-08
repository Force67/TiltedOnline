#pragma once

#include "BSCore/BSFixedString.h"

namespace creation
{
    namespace BSScript
    {
        class ObjectTypeInfo
        {
            int64_t GetVariableIndex(BSFixedString* aName) noexcept;

            uint8_t pad0[0x10];
            BSFixedString name;
            uint8_t pad18[0x28];
            uint64_t flags1;
            uint8_t pad48[0x8];
            void* data;
        };
        static_assert(sizeof(ObjectTypeInfo) == 0x58);
    }
}
