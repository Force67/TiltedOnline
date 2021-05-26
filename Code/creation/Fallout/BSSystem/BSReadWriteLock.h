#pragma once

#include <stdafx.h>

namespace creation
{
    class BSReadWriteLock
    {
        uint32_t m_uiOperatingThread;
        std::atomic<uint32_t> m_uiLockCount;
    };
}
