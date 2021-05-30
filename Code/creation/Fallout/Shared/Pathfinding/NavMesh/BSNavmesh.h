#pragma once

#include <stdafx.h>

#include "Shared/FormComponents/TESForm.h"
#include "BSSystem/BSTSmartPointer.h"

namespace creation
{
    class BSNavmesh : public BSIntrusiveRefCounted
    {
        virtual ~BSNavmesh();
        virtual uint32_t sub_1();

        uint8_t pad0[0x4];
        uint8_t pad4[0x1F8];
    };

    static_assert(sizeof(BSNavmesh) == 0x208);
}
