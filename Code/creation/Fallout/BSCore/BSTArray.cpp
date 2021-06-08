
#include "BSCore/BSTArray.h"

namespace creation
{
    bool BSTArrayHeapAllocator::Allocate(uint32_t auiMinSize, uint32_t auiSize)
    {
        const uint32_t minSize = auiMinSize > 4 ? auiMinSize : 4;
        const size_t allocSize = static_cast<size_t>(auiSize) * minSize;

        m_pBuffer = new uint8_t[allocSize];
        if (m_pBuffer)
        {
            m_uiBufferSize = static_cast<uint32_t>(allocSize);
        }

        return m_pBuffer;
    }

    void BSTArrayHeapAllocator::Deallocate()
    {
        delete[] m_pBuffer;

        m_pBuffer = nullptr;
        m_uiBufferSize = 0;
    }

    bool BSTArrayBase::SetAllocSize(IAllocatorFunctor* aFunctor, uint32_t auiAllocSize, uint32_t auiNewAllocSize, uint32_t auiElemSize)
    {
        const uint32_t minSize = auiNewAllocSize < m_uiSize ? m_uiSize : auiNewAllocSize;
        if (minSize == auiAllocSize)
            return true;

        if (!minSize)
        {
            aFunctor->Deallocate();
            return false;
        }

        if (minSize <= auiAllocSize)
            return true;

        if (auiAllocSize)
            return aFunctor->Reallocate(minSize, m_uiSize, 0, 0, auiElemSize);
        else
            return aFunctor->Allocate(minSize, auiElemSize);
    }
}
