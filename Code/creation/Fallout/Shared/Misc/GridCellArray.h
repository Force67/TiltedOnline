#pragma once

#include "GridArray.h"

namespace creation
{
    class TESObjectCELL;

    class GridCellArray : public GridArray
    {
    public:
        virtual ~GridCellArray();

        virtual void UnloadAll() override;
        virtual void sub_02() override;
        virtual void SetCenter(int aX, int aY) override;
        virtual void ProcessDeltaChange(int aOffsetX, int aOffsetY) override;
        virtual void UnloadOffset(int aOffsetX, int aOffsetY) override;
        virtual void LoadOffset(int aOffsetX, int aOffsetY) override;
        virtual void MoveCell(int aFromX, int aFromY, int aToX, int aToY) override;
        virtual void SwapCells(int aFirstX, int aFirstY, int aSecondX, int aSecondY) override;

        TESObjectCELL** m_gridCells;
        uint8_t pad20[0x68 - 0x20];
    };

    static_assert(sizeof(GridCellArray) == 0x68);
}
