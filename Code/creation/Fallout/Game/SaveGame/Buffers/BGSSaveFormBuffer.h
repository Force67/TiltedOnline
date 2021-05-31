#pragma once

namespace creation
{
    class BGSSaveFormBuffer
    {
    public:
        BGSSaveFormBuffer();
        virtual ~BGSSaveFormBuffer() {}

        void WriteId(uint32_t aId) noexcept;

        char* m_pBuffer;
        uint32_t m_uiCapacity;
        uint32_t m_uiPosition;
        uint8_t m_uiFormId24[3];
        uint32_t m_uiChangeFlags;
        uint8_t pad[0xC];
    };

    static_assert(sizeof(BGSSaveFormBuffer) == 0x30);
}
