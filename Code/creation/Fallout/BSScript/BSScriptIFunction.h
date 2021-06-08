#pragma once

#include "BSSystem/BSTSmartPointer.h"

namespace creation
{
    namespace BSScript
    {
        class IFunction : public BSIntrusiveRefCounted
        {
        protected:
            virtual ~IFunction();

            uint8_t pad4[0x4];
        };

        static_assert(sizeof(IFunction) == 0x10);
    }
}
