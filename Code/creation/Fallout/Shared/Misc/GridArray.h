#pragma once

namespace creation
{
    class GridArray
    {
        virtual ~GridArray();
        virtual void UnloadAll();
        virtual void sub_02();
        virtual void SetCenter(int aX, int aY);
        virtual void ProcessDeltaChange(int aOffsetX, int aOffsetY);
        virtual void UnloadOffset(int aOffsetX, int aOffsetY);
        virtual void LoadOffset(int aOffsetX, int aOffsetY);
        virtual void MoveCell(int aFromX, int aFromY, int aToX, int aToY);
        virtual void SwapCells(int aFirstX, int aFirstY, int aSecondX, int aSecondY);

        int32_t m_iCenterX;
        int32_t m_iCenterY;
        uint32_t m_uiDimension;
        uint8_t pad14[0x4];
    };

    static_assert(sizeof(GridArray) == 0x18);
}
