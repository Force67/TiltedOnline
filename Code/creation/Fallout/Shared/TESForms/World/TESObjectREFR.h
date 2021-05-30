#pragma once

#include "Shared/FormComponents/TESForm.h"
#include "Shared/ExtraData/ExtraDataList.h"
#include "BSSystem/BSTSmartPointer.h"
#include "BSMain/BSHandleRefObject.h"
#include "TESObjectCELL.h"
#include "BSMain/BSHandleRefObject.h"
#include "Shared/Animation/IAnimationGraphManagerHolder.h"
#include "Shared/FormComponents/IKeywordFormBase.h"
#include "Shared/AI/ActorValueOwner.h"
#include "NetImmerse/NiPoint3.h"

namespace creation
{
    class TESObjectREFR : public TESForm, public BSHandleRefObject, public IAnimationGraphManagerHolder,
        public IKeywordFormBase, public ActorValueOwner
    {
    public:
        TESObjectCELL* m_pParentCell;
        NiPoint3 m_rotation;
        uint32_t padCC;
        NiPoint3 m_position;
        uint32_t padDC;
        TESForm* m_baseForm;
        uint8_t padE8;
        void* m_pLoadedSTate;
        void* m_pInventory;
        BSTSmartPointer<ExtraDataList> m_pExtraData;
        uint16_t m_usReferenceScale;
        uint8_t m_eModelState;
        bool m_bDestroying;
    };
    constexpr size_t t = offsetof(TESObjectREFR, m_pParentCell);
    constexpr size_t t = sizeof(TESObjectREFR);

    static_assert(sizeof(TESObjectREFR) == 0x110);
}
