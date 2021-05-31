#pragma once

template <class T> class BSTEventSink;

#include <stdafx.h>
#include "BSCore/BSFixedString.h"

class ActorValueInfo;
class TESObjectREFR;

namespace creation
{
    // Very nasty work around to avoid template code duplication
    namespace details
    {
        void InternalRegisterSink(void* apEventDispatcher, void* apSink) noexcept;
        void InternalUnRegisterSink(void* apEventDispatcher, void* apSink) noexcept;
        void InternalPushEvent(void* apEventDispatcher, void* apEvent) noexcept;
    }

    template<class T>
    class BSTEventSource
    {
      public:
        void RegisterSink(BSTEventSink<T>* apSink) noexcept
        {
            details::InternalRegisterSink(reinterpret_cast<void*>(this), reinterpret_cast<void*>(apSink));
        }

        void UnRegisterSink(BSTEventSink<T>* apSink) noexcept
        {
            details::InternalUnRegisterSink(reinterpret_cast<void*>(this), reinterpret_cast<void*>(apSink));
        }

        void PushEvent(const T* apEvent) noexcept
        {
            details::InternalPushEvent(reinterpret_cast<void*>(this), reinterpret_cast<void*>(apEvent));
        }

        uint8_t pad0[0x58];
    };

    class BSActiveGraphIfInactiveEvent
    {
    };

    class BSAnimationGraphEvent
    {
    };

    class BGSInventoryListEvent
    {
    public:
        enum Type : int16_t
        {
            kAddNewStack,
            kChangedStack,
            kAddNewItem,
            kRemoveItem,
            kClear,
            kChangeWeight
        };

        class Event
        {
            Type m_uiEventType;
            uint32_t m_uiOwnerBits;
            void* m_pChangedObject; // TODO: change void to type (TESBoundObject)
            uint64_t unk10;
        };
    };

    class ActorValueChangedEvent
    {
        ActorValueInfo* m_pActorValueType;
        TESObjectREFR* m_pActorValueOwner;
    };

    class BSMovementDataChangedEvent
    {
    };

    class BSTransformDeltaEvent
    {
    };

    class BSSubGraphActivationUpdate
    {
    };

    class bhkCharacterMoveFinishEvent
    {
    };

    class bhkNonSupportContactEvent
    {
    };

    class bhkCharacterStateChangeEvent
    {
    };

    class MovementMessageUpdateRequestImmediate
    {
    };

    class PerkValueEvents
    {
    public:
        enum Type : int16_t
        {
            kAdd,
            kRemove
        };

        class PerkValueChangedEvent
        {
            Type m_uiType;
            uint32_t m_uiOwnerBits;
            void* m_pPerk; // TODO: change void to type (BGSPerk)
            uint8_t unk10;
        };

        class PerkEntryUpdatedEvent
        {
            uint32_t m_uiOwnerBits;
            void* m_pPerkEntry; // TODO: change void to type (BGSPerkEntry)
        };
    };

    class ActorCPMEvent
    {
    };

    class MenuOpenCloseEvent
    {
    };
    
    class MenuModeChangeEvent
    {
    };

    class UserEventEnabledEvent
    {
    };

    class OtherEventEnabledEvent 
    {
    };

    class TESHitEvent
    {
    };

    class BGSActorCellEvent
    {
    };

    class BGSActorDeathEvent
    {
    };

    class PositionPlayerEvent
    {
    };
    
    class PickRefUpdateEvent
    {
    };

}
