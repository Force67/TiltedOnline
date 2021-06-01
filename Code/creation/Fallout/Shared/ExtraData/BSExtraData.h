#pragma once

namespace creation
{
    class BSExtraData
    {
    public:
        virtual ~BSExtraData() = default;
        virtual bool sub_1();
        virtual bool sub_2();

        BSExtraData *m_pNextEntry;
        uint16_t m_uiFlags;
        uint8_t m_uiType;
    };

    static_assert(sizeof(BSExtraData) == 0x18);
}

