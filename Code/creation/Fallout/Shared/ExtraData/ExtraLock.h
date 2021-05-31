#pragma once

#include "BSExtraData.h"
#include "Lock.h"

namespace creation
{
    class ExtraLock : public BSExtraData
    {
        Lock* m_pLock;
    };
}
