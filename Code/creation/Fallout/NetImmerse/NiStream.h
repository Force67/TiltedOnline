#pragma once

#include <atomic>

namespace creation
{
    class NiStream
    {
    public:
        NiStream();

    private:
    };
    static_assert(sizeof(NiRefObject) == 0x10);
} // namespace creation
