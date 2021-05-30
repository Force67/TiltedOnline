#pragma once

#include <cstdint>

namespace creation
{
    template <class T>
    class BSTSmallArray
    {
        int32_t capacity; // 0
        T** data;         // 8
        uint32_t size;    // 10

        T* Get(uint32_t aIndex)
        {
            auto pData = reinterpret_cast<T**>(&data);
            if (capacity >= 0)
                pData = data;

            return pData[aIndex];
        }
    };
} // namespace creation
