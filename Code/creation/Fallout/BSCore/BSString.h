#pragma once

#include <cstdint>

namespace creation
{
    template <typename T>
    struct BSStringT
    {
        T* buffer;
        uint16_t length;
        uint16_t capacity;

        // inlined by the game.
        inline uint16_t GetLength() const
        {
            if (length == 0xFFFF)
            {
                if constexpr (std::is_same<T, wchar_t>())
                    return std::wcslen(buffer);

                if constexpr (std::is_same<T, char>())
                    return std::strlen(buffer);
            }

            return length;
        }

        // inlined by the game.
        inline const T* c_str() const
        {
            return buffer;
        }
    };

    using BSStringA = BSStringT<char>;
    using BSStringW = BSStringT<wchar_t>;

    static_assert(sizeof(BSStringT<void>) == 0x10);
} // namespace creation
