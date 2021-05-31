#pragma once

namespace creation
{
    class TESObjectCELL;
    class GridCellArray;

    class TES
    {
    public:
        static TES* Get() noexcept;

        uint8_t pad[0x18];
        GridCellArray* m_pCells;
        uint8_t pad20[0x48 - 0x20];
        int32_t m_uiCenterGridX;
        int32_t m_uiCenterGridY;
        int32_t m_uiCurrentGridX;
        int32_t m_uiCurrentGridY;
        TESObjectCELL* m_pInteriorCell;
        uint8_t pad60[0x138 - 0x60];
    };

    static_assert(offsetof(TES, m_pCells) == 0x18);
    static_assert(offsetof(TES, m_pInteriorCell) == 0x58);
    static_assert(sizeof(TES) == 0x138);
}
