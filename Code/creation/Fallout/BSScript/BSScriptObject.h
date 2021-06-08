#pragma once

#include "BSCore/BSFixedString.h"
#include "BSScriptVariable.h"

namespace creation
{
    namespace BSScript
    {
        class ObjectTypeInfo;

        class Object
        {
            void IncreaseRef() noexcept;
            void DecreaseRef() noexcept;

            uint8_t pad0[0x8];
            ObjectTypeInfo* typeInfo;
            BSFixedString state;
            uint8_t pad18[0x30 - 0x18];
            Variable variables[1]; // variables are stored like BSFixedString stores characters
        };
        static_assert(sizeof(Object) == 0x40);
    }
}
