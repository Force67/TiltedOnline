#pragma once

#include <cstdint>

namespace creation
{
    class BSTArrayHeapAllocator
    {
        void* m_pBuffer;
        uint32_t m_uiSize;
    };

    template <class T>
    struct BSTArray
    {
        T* data;
        alignas(sizeof(void*)) uint32_t capacity; // Might be size_t
        alignas(sizeof(void*)) uint32_t length;   // Might be size_t

        BSTArray() : data{ nullptr }, capacity{}, length{ 0 }
        {
        }
        ~BSTArray() = default;

        T& operator[](uint32_t aIndex)
        {
            return data[aIndex];
        }
        const T& operator[](uint32_t aIndex) const
        {
            return data[aIndex];
        }

        // Range for loop compatibility
        struct Iterator
        {
            Iterator(T* apEntry) : m_pEntry(apEntry)
            {
            }
            Iterator operator++()
            {
                ++m_pEntry;
                return *this;
            }
            bool operator!=(const Iterator& acRhs) const
            {
                return m_pEntry != acRhs.m_pEntry;
            }
            const T& operator*() const
            {
                return *m_pEntry;
            }

        private:
            T* m_pEntry;
        };

        Iterator begin()
        {
            return Iterator(&data[0]);
        }

        Iterator end()
        {
            return Iterator(&data[length]);
        }

        inline bool Empty() const noexcept
        {
            return length == 0;
        }
    };
} // namespace creation
