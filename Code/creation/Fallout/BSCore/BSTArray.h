#pragma once

#include <cstdint>

namespace creation
{
    class BSTArrayHeapAllocator
    {
    public:
        inline ~BSTArrayHeapAllocator()
        {
            if (m_pBuffer)
                Deallocate();

            m_uiBufferSize = 0;
        }

        bool Allocate(uint32_t auiMinSize, uint32_t auiSize);
        void Deallocate();
        bool Reallocate(unsigned int aiMinNewSizeInItems, unsigned int aiFrontCopyCount, unsigned int aiShiftCount, unsigned int aiBackCopyCount, unsigned int aiElemSize) {
            __debugbreak();
            return false;
        }

        // TODO: impl Reallocate
 
        inline void* GetBuffer() const;
        inline uint32_t GetBufferSize() const;

    protected:
        void* m_pBuffer = nullptr;
        uint32_t m_uiBufferSize = 0;
    };

    static_assert(sizeof(BSTArrayHeapAllocator) == 0x10);

    class BSTArrayBase
    {
    public:
        inline ~BSTArrayBase()
        {
            m_uiSize = 0;
        }

        struct IAllocatorFunctor
        {
            virtual bool Allocate(uint32_t auiSize, uint32_t auiItemSize) = 0;
            virtual bool Reallocate(uint32_t auiMinSize, uint32_t aUnk2, uint32_t aUnk3, uint32_t aUnk4, uint32_t auiItemSize) = 0;
            virtual void Deallocate() = 0;
            // dtor comes last:
            virtual ~IAllocatorFunctor() {};
        };
        
        bool InitialReserve(IAllocatorFunctor* arFunctor, uint32_t auiReserveSize, uint32_t auiElemSize)
        {
            if (auiReserveSize)
                return arFunctor->Allocate(auiReserveSize, auiElemSize);
            else
                return true;
        }

        bool SetAllocSize(IAllocatorFunctor* arFunctor, uint32_t auiAllocSize, uint32_t auiNewAllocSize, uint32_t auiElemSize);

    private:
        uint32_t m_uiSize = 0;
    };

    template<class TAllocator>
    struct BSTArrayAllocatorFunctor : public BSTArrayBase::IAllocatorFunctor
    {
        inline BSTArrayAllocatorFunctor(TAllocator *apAllocator) : pAllocator(apAllocator) {}

        bool Allocate(uint32_t auiSize, uint32_t auiItemSize) override { return pAllocator->Allocate(auiSize, auiSize); }
        bool Reallocate(unsigned int auiMinNewSizeInItems, unsigned int auiFrontCopyCount, unsigned int auiShiftCount, unsigned int auiBackCopyCount, unsigned int auiElemSize) override
        {
            return pAllocator->Reallocate(auiMinNewSizeInItems, auiFrontCopyCount, auiShiftCount, auiBackCopyCount, auiElemSize);
        }

        void Deallocate() override {
            pAllocator->Deallocate();
        }

        ~BSTArrayAllocatorFunctor() override {
            delete this;
        }

        TAllocator* pAllocator;
    };

    static_assert(sizeof(BSTArrayBase) == 4);

    template<class T, class Allocator = BSTArrayHeapAllocator>
    class BSTArray final : public Allocator
    {
    public:
        using Functor_t = BSTArrayAllocatorFunctor<Allocator>;

        explicit BSTArray(uint32_t aiReserveSize)
        {
            Functor_t functor(this);
            arrayBase.InitialReserve(&functor, aiReserveSize, sizeof(T));
        }

        BSTArray() = default;

        // TODO: add logic.

        void SetAllocSize(uint32_t aSize)
        {
            Functor_t functor(this);
            arrayBase.SetAllocSize(&functor, m_uiBufferSize, aSize, sizeof(T));
        }

    public:
        BSTArrayBase arrayBase;
    };

    static_assert(sizeof(BSTArray<void>) == 0x18);

    template <class T>
    struct BSTArray2
    {
        T* data;
        alignas(sizeof(void*)) uint32_t capacity; // Might be size_t
        alignas(sizeof(void*)) uint32_t length;   // Might be size_t

        BSTArray2() : data{ nullptr }, capacity{}, length{ 0 }
        {
        }
        ~BSTArray2() = default;

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
