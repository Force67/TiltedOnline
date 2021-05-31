#pragma once

#include "Actor.h"

#include "Movement/IMovementPlayerControlsFilter.h"

namespace creation
{
    class TESQuest;

    class PlayerCharacter : public Actor, public BSTEventSink<MenuOpenCloseEvent>, 
                            public BSTEventSink<MenuModeChangeEvent>, public BSTEventSink<UserEventEnabledEvent>, 
                            public BSTEventSink<OtherEventEnabledEvent>, public BSTEventSink<TESHitEvent>, 
                            public BSTEventSink<PerkValueEvents::PerkEntryUpdatedEvent>, public IMovementPlayerControlsFilter, 
                            public BSTEventSource<BGSActorCellEvent>, public BSTEventSource<BGSActorDeathEvent>, 
                            public BSTEventSource<PositionPlayerEvent>, public BSTEventSource<PickRefUpdateEvent>
    {
    public:
        static PlayerCharacter* Get() noexcept;

        struct Objective
        {
            uint64_t pad0;
            TESQuest* quest;
        };

        struct ObjectiveInstance
        {
            Objective* instance;
            uint64_t instanceCount;
        };

        uint8_t pad490[0x7D8 - 0x628];
        BSTArray<ObjectiveInstance> m_objectives;
        uint8_t pad7F0[0x4F0 - 24];
        TESForm* m_pLocationForm; //actually a bgslocation
        uint8_t pad_CD0[0x140];
    };

    static_assert(sizeof(PlayerCharacter) == 0xE10);
    static_assert(offsetof(PlayerCharacter, m_objectives) == 0x7D8);
    static_assert(offsetof(PlayerCharacter, m_pLocationForm) == 0xCC8);
}
