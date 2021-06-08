#pragma once

#include <atomic>

namespace creation
{
    struct BSIntrusiveRefCounted
    {
    public:
        template <class T>
        friend struct BSTSmartPointerIntrusiveRefCount;

        inline BSIntrusiveRefCounted() : uiRefCount(0) { }

        void IncRef() { uiRefCount++; }
        void DecRef() {
            uiRefCount--;
            if (uiRefCount == 0)
                delete this;
        }

    protected:
        // should be BSTAtomic something
        std::atomic<uint32_t> uiRefCount;
    };


    template <class T>
    struct BSTSmartPointerIntrusiveRefCount
    {
        static void Acquire(T* p) { (BSIntrusiveRefCounted*)p->IncRef(); }
        static void Release(T* p) { (BSIntrusiveRefCounted*)p->DecRef(); }
    };

    template <class T, template <class> class RefManager = BSTSmartPointerIntrusiveRefCount>
    class BSTSmartPointer
    {
    public:
        BSTSmartPointer() : pPtr(nullptr) {}
        BSTSmartPointer(const BSTSmartPointer& aRhs) : pPtr(nullptr)
        {
            Acquire(aRhs);
        }
        BSTSmartPointer(const T* apRhs) : pPtr(nullptr)
        {
            Acquire(apRhs);
        }
        BSTSmartPointer(BSTSmartPointer&& aRhs)
        {
            pPtr = aRhs.pPtr;
            aRhs.pPtr = nullptr;
        }

        ~BSTSmartPointer() { Release(); }

        operator T*() { return pPtr; }
        operator const T*() const { return pPtr; }
        operator bool() const { return pPtr != 0; }
        T* operator->() { return pPtr; }
  
        BSTSmartPointer& operator=(const T* apRhs) 
        {
            Release();
            Acquire(apRhs);
            return *this;
        }

        BSTSmartPointer& operator=(const BSTSmartPointer& aRhs) 
        {
            Release();
            Acquire(&aRhs);
            return *this;
        }

        BSTSmartPointer& operator=(BSTSmartPointer&& aRhs)
        {
            Release();
            pPtr = aRhs.pPtr;
            aRhs.pPtr = nullptr;
            return *this;
        }

        void Acquire(const T* aRhs)
        {
            if (pPtr = const_cast<T*>(aRhs))
            {
                RefManager<T>::Acquire(pPtr);
            }
        }

        void Release(void) 
        {
            if (pPtr) 
            {
                RefManager<T>::Release(pPtr);
                pPtr = nullptr;
            }
        }

    protected:
        T* pPtr;
    };
}