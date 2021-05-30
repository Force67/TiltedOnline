#pragma once

#include "BSCore/BSStringPool.h"

namespace creation
{
    class BSFixedString
    {
    public:
        BSFixedString();
        ~BSFixedString();

        bool operator!()
        {
            return m_pEntry == nullptr;
        }

    private:
        BSStringPool::Entry* m_pEntry = nullptr;
    };

    static_assert(sizeof(BSFixedString) == 8);
} // namespace creation
