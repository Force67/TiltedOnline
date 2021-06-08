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
} // namespace creation
