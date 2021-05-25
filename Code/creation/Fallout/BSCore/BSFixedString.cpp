
#include "BSCore/BSFixedString.h"

namespace creation
{
    static BSFixedString s_EmptryString{};
    
    BSFixedString::BSFixedString()
    {
        m_pEntry = s_EmptryString.m_pEntry;
    }
    
    BSFixedString::~BSFixedString()
    {
    // BSStringPool::Entry::Release(&m_pEntry);
    }
} // namespace creation
