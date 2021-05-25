#pragma once

#include <cstdint>

namespace creation
{
    template <class T> 
    struct BSList
    {
        struct Entry
        {
            T* data;
            Entry* next;
        };

        Entry entry;

        inline bool Empty() const noexcept
        {
            return entry.data == nullptr;
        }

        // Range for loop compatibility
        struct Iterator
        {
            Iterator(Entry* apEntry) : m_pEntry(apEntry)
            {
            }
            Iterator operator++()
            {
                m_pEntry = m_pEntry->next;
                return *this;
            }
            bool operator!=(const Iterator& acRhs) const
            {
                return m_pEntry != acRhs.m_pEntry;
            }
            T* operator*() const
            {
                return m_pEntry->data;
            }

          private:
            Entry* m_pEntry;
        };

        Iterator begin()
        {
            return Iterator(&entry);
        }

        Iterator end()
        {
            return Iterator(nullptr);
        }
    };
} // namespace creation
