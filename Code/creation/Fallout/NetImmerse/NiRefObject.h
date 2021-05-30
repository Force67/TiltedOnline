#pragma once

#include <atomic>

namespace creation
{
    class NiRefObject
    {
    public:
        NiRefObject();
        virtual ~NiRefObject();

        virtual void DeleteThis();

        void IncRefCount();
        void DecRefCount();

    private:
        uint32_t m_uiRefCount;
        uint32_t m_pad1;
        static std::atomic<uint32_t> ms_uiObjects;
};

static_assert(sizeof(NiRefObject) == 0x10);
} // namespace creation
