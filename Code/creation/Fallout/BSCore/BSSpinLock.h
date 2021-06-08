#pragma once

#include <stdafx.h>

namespace creation
{
    class BSSpinLock
    {
        void Lock() noexcept;
        void Unlock() noexcept;

        uint32_t m_uiOperatingThread;
        std::atomic<uint32_t> m_uiLockCount;
    };
}
