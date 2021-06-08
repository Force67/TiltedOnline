
#include "BSTArray.h"

namespace creation
{
    void* BSTArrayHeapAllocator::GetBuffer() const
    {
        return m_pBuffer;
    }

    inline uint32_t BSTArrayHeapAllocator::GetBufferSize() const
    {
        return m_uiBufferSize;
    }
}
