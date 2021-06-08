#pragma once

#include "BSCore/BSTArray.h"

namespace creation
{
    class ProcessLists
    {
    public:
        static ProcessLists* Get() noexcept;

        uint8_t pad0[0x40];
        BSTArray<uint32_t> m_actorProcessLists[4]; // 0 is highProcessHandles
        uint8_t padA0[0x218 - 0xA0];
    };

    static_assert(offsetof(ProcessLists, m_actorProcessLists) == 0x40);
    static_assert(sizeof(ProcessLists) == 0x218);
}
