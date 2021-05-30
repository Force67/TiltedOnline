#pragma once

namespace creation
{
    template <class T>
    class BSSimpleList
    {
    public:
        inline BSSimpleList() = default;
        inline BSSimpleList(T apItem)
        {
            m_item = apItem;
        }

        // Bethesda's weird linked list.
        inline void AddTail(T apItem)
        {
            if (BSSimpleList<T>** it = &m_pkNext)
            {
                do 
                {
                    it = &(*it)->m_pkNext;
                } while (&(*it)->m_pkNext);

                auto* newItem = new BSSimpleList<T>(apItem);
                newItem->m_pkNext = nullptr;

                (*it)->m_pkNext = newItem;
            }
        }

        inline BSSimpleList<T>* GetTail()
        {
            if (BSSimpleList<T>** it = &m_pkNext)
            {
                do
                {
                    it = &(*it)->m_pkNext;
                } while (&(*it)->m_pkNext);

                return (*it);
            }

            return this;
        }

        inline void RemoveAll()
        {
            __debugbreak();

            /*if (m_item)
                delete m_item;
            if (BSSimpleList<T>** it = &m_pkNext)
            {
                do
                {
                    delete it->m_item;

                    auto* save = it->m_pkNext;
                    it->m_pkNext = nullptr;
                    it = save;

                } while (it->m_pkNext);

                return it;
            }*/
        }

        inline bool IsEmpty() const { return !m_pkNext && !m_item; }
        inline auto* GetNext() const { return m_pkNext; }
        inline const T GetItem() const { return m_item; }

    public:
        T m_item{};
        BSSimpleList<T>* m_pkNext = nullptr;
    };

    static_assert(sizeof(BSSimpleList<uintptr_t>) == 0x10);
}
