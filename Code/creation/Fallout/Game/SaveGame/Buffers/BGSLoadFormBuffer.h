#pragma once

namespace creation
{
    class BGSLoadFormBuffer
    {
    public:
        BGSLoadFormBuffer(uint32_t aChangeFlags);
        virtual ~BGSLoadFormBuffer() {}

        void SetSize(const uint32_t aSize) noexcept
        {
            capacity = size1 = aSize;
            size2 = 0;
        }

        const char* buffer; // 8
        struct ScrapHeap* scrapHeap; // 10
        uint32_t capacity; // 18
        uint32_t unk1C; // 1C - set to 1
        uint32_t size1; // 20
        uint32_t position; // 24
        uint32_t formId; // 28
        uint32_t size2; // 2C
        void* unk30; // 30
        struct TESForm* form; // 38
        uint32_t changeFlags; // 40
        uint32_t maybeMoreFlags; // 44 ??????
        uint8_t unk48; // 48 - some flags apparently
        uint8_t unk49;
        uint8_t unk4A;
        uint8_t loadFlag;
        uint8_t pad[0x50];
    };
}
