#pragma once

#include <cstdint>

namespace creation
{
// Pool has no size, it acts as a static
// interface
struct BSStringPool
{
    struct Entry
    {
        void Acquire();
        void Release();

        Entry* pLeft;
        uint32_t flags;
        uintptr_t lengthptr;
    };
};

static_assert(sizeof(BSStringPool::Entry) == 0x18);
} // namespace creation
