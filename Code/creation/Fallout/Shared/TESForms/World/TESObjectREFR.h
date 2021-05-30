#pragma once

#include "Shared/FormComponents/TESForm.h"
#include "Shared/ExtraData/ExtraDataList.h"
#include "BSSystem/BSTSmartPointer.h"
#include "BSMain/BSHandleRefObject.h"
#include "BSMain/BSHandleRefObject.h"
#include "Shared/Animation/IAnimationGraphManagerHolder.h"
#include "Shared/FormComponents/IKeywordFormBase.h"
#include "Shared/AI/ActorValueOwner.h"
#include "NetImmerse/NiPoint3.h"
#include "Shared/Events/BSTEventSink.h"

class TESObjectCELL;

namespace creation
{
    class TESObjectREFR : public TESForm, public BSHandleRefObject, public BSTEventSink<BSActiveGraphIfInactiveEvent>,
                          public BSTEventSink<BSAnimationGraphEvent>, public BSTEventSink<BGSInventoryListEvent::Event>,
                          public IAnimationGraphManagerHolder, public IKeywordFormBase, public ActorValueOwner,
                          public BSTEventSource<ActorValueChangedEvent>
    {
    public:
        TESObjectCELL* m_pParentCell;
        NiPoint3 m_rotation;
        uint32_t padCC;
        NiPoint3 m_position;
        uint32_t padDC;
        TESForm* m_pBaseForm;
        uint8_t padE8;
        void* m_pLoadedSTate;
        void* m_pInventory;
        BSTSmartPointer<ExtraDataList> m_pExtraData;
        uint8_t pad108[0x4];
    };

    static_assert(offsetof(TESObjectREFR, m_pBaseForm) == 0xE0);
    static_assert(offsetof(TESObjectREFR, m_position) == 0xD0);
    static_assert(offsetof(TESObjectREFR, m_rotation) == 0xC0);
    static_assert(offsetof(TESObjectREFR, m_pParentCell) == 0xB8);
    static_assert(offsetof(TESObjectREFR, m_pInventory) == 0xF8);
    static_assert(offsetof(TESObjectREFR, m_pExtraData) == 0x100);
    static_assert(sizeof(TESObjectREFR) == 0x110);
}
