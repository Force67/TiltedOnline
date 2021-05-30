#pragma once

#include "Shared/FormComponents/TESForm.h"
#include "BSCore/BSSpinLock.h"
#include "Shared/FormComponents/TESFullName.h"
#include "BSCore/BSTArray.h"
#include "NetImmerse/NiPointer.h"
#include "Shared/Pathfinding/NavMesh/NavMeshArray.h"
#include "TESWorldSpace.h"

namespace creation
{
    class TESObjectCELL : public TESForm, public TESFullName
    {
    public:
        virtual ~TESObjectCELL() = default;

        BSSpinLock m_lock;
        BSSpinLock m_lock2;
        uint16_t m_uiCellFlags;
        uint16_t m_uiCellFlags2;
        uint8_t m_uiCellState;
        bool m_bIsWaterLoaded;
        bool m_bIsCellDetached;
        BSTSmartPointer<ExtraDataList,BSTSmartPointerIntrusiveRefCount> m_extraData;
        uint8_t unk50[0x8];
        void* unkPtr58;
        float m_fSeaLevel;
        NavMeshArray* m_pNavMeshArray;
        BSTArray<NiPointer<TESObjectREFR>> m_pRefArray; // missing BSTArrayHeapAllocator
        void* unkPtr88;
        uint8_t unk90[0x30];
        BSSpinLock m_lock3;
        union {
            TESWorldSpace* m_pWorldSpace;
            uint32_t m_uiDataOffset;
        };
        void* unktPtrD0;
        void* unkPtrD8;
        uint8_t unkE0[0x10];
    };

    static_assert(sizeof(TESObjectCELL) == 0xF0);
}

