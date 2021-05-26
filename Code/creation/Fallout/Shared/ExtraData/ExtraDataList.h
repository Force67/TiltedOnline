#pragma once

#include <stdafx.h>

#include "ExtraList.h"
#include "BSSystem/BSReadWriteLock.h"

namespace creation
{
    class ExtraDataList
    {
        uint32_t m_uiRefCount;
        ExtraList m_extraList;
        BSReadWriteLock m_Lock;
    };
}
