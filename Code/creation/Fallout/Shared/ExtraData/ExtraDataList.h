#pragma once

#include "BaseExtraList.h"
#include "BSSystem/BSReadWriteLock.h"

namespace creation
{
    class ExtraDataList
    {
        uint32_t m_uiRefCount;
        BaseExtraList m_extraList;
        BSReadWriteLock m_Lock;
    };
}
