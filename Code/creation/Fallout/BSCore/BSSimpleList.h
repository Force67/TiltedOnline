#pragma once

namespace creation
{
    template<class T>
    class BSSimpleList
    {
    public:
        struct Entry
        {
            T* m_data;
            Entry* m_pNext;
        };

        Entry m_entry;

        inline bool Empty() const noexcept
        {
            return m_entry.m_data == nullptr;
        }

        // Range for loop compatibility
        struct Iterator 
        {
            Iterator(Entry* apEntry) : m_pEntry(apEntry) {}
            Iterator operator++() { m_pEntry = m_pEntry->m_pNext; return *this; }
            bool operator!=(const Iterator& acRhs) const { return m_pEntry != acRhs.m_pEntry; }
            T* operator*() const { return m_pEntry->m_data; }
        private:
            Entry* m_pEntry;
        };

        Iterator begin() 
        {
            return Iterator(&m_entry);
        }

        Iterator end()
        {
            return Iterator(nullptr);
        }
    };
}
