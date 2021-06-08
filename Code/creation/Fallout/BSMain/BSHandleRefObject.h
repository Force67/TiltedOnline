#pragma once

#include <stdafx.h>

#include "NetImmerse/NiRefObject.h"

namespace creation
{
    class BSHandleRefObject : public NiRefObject
    {
        void DecRefHandle()
        {
            if ((InterlockedDecrement(&m_uiRefCount) & 0x3FF) == 0)
                DeleteThis();
        }
    };
}
