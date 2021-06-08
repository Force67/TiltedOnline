#pragma once

#include "NetImmerse/NiPoint3.h"
#include "BSCore/BSSimpleList.h"
#include "TESFile.h"

namespace creation
{
    class TESObjectCELL;
    class TESWorldSpace;
    class Actor;

    class TESDataHandler
    {
    public:
        static TESDataHandler* Get() noexcept;

        uint32_t Spawn(NiPoint3& aPosition, NiPoint3& aRotation, TESObjectCELL* apParentCell, TESWorldSpace* apWorldSpace, Actor* apCharacter) noexcept;
        TESFile* GetByName(const char* acpName) const noexcept;
        TESObjectCELL* GetCellFromCoordinates(int32_t aX, int32_t aY, TESWorldSpace* aWorldSpace, bool aSpawnCell) noexcept;

        uint8_t pad0[0xFB0];
        BSSimpleList<TESFile*> m_files;
        uint8_t padFC0[0x1020 - 0xFC0];
    };

    static_assert(sizeof(TESDataHandler) == 0x1020);
}
