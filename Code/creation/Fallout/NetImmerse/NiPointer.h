#pragma once

#include "NetImmerse/NiRefObject.h"

namespace creation
{
template <typename T> class NiPointer
{
  public:
    NiRefObject() : m_pObject(nullptr)
    {
    }

    NiPointer(T* pObject) : m_pObject(pObject)
    {
        m_pObject->IncRefCount();
    }

    ~NiPointer()
    {
        m_pObject->DecRefCount();
    }

    void Acquire(NiRefObject* pObjectRef)
    {
        pObjectRef->IncRefCount();
    }

    void Release(NiRefObject* pObjectRef)
    {
        pObjectRef->DecRefCount();
    }

    NiPointer<NiRefObject> operator=(NiRefObject* pRhs)
    {
        if (m_pObject != pRhs)
        {
            pRhs->IncRefCount();

            m_pObject = pRhs;

            if (m_pObject)
                m_pObject->DecRefCount();
        }

        return this;
    }

    bool operator!=(NiRefObject* pRhs)
    {
        return m_pObject != pRhs;
    }

    NiRefObject* getPtr() const
    {
        return m_pObject;
    }

  private:
    T* m_pObject;
};

// ensure that it aligns to 8
// static_assert(sizeof(NiPointer<NiRefObject>) == 8);
} // namespace creation
