#pragma once

#include "Shared/TESForms/World/TESObjectREFR.h"
#include "Shared/Magic/MagicTarget.h"
#include "ActorState.h"
#include "Shared/Animation/IPostAnimationChannelUpdateFunctor.h"

#include <Structs/Inventory.h>
#include <Structs/Factions.h>
#include <Structs/ActorValues.h>

namespace creation
{
    class TESNPC;
    class ActorExtension;
    class ExActor;
    class ExPlayerCharacter;
    class AIProcess;
    class TESFaction;

    class Actor : public TESObjectREFR, public MagicTarget, public ActorState,
                  public BSTEventSink<BSMovementDataChangedEvent>, public BSTEventSink<BSTransformDeltaEvent>,
                  public BSTEventSink<BSSubGraphActivationUpdate>, public BSTEventSink<bhkCharacterMoveFinishEvent>,
                  public BSTEventSink<bhkNonSupportContactEvent>, public BSTEventSink<bhkCharacterStateChangeEvent>,
                  public IPostAnimationChannelUpdateFunctor, public BSTEventSource<MovementMessageUpdateRequestImmediate>,
                  public BSTEventSource<PerkValueEvents::PerkValueChangedEvent>, public BSTEventSource<PerkValueEvents::PerkEntryUpdatedEvent>,
                  public BSTEventSource<ActorCPMEvent>
    {
    public:
        static constexpr uint32_t Type = FormType::kCharacter;

        static BSTSmartPointer<Actor> New() noexcept;
        static BSTSmartPointer<Actor> Create(TESNPC* apNpc) noexcept;

        virtual ~Actor() override;

        virtual void sub_C4();
        virtual void sub_C5();
        virtual void sub_C6();
        virtual void sub_C7();
        virtual void sub_C8();
        virtual void SetWeaponDrawn(bool aIsDrawn);
        virtual void SetPosition(const NiPoint3& acPosition, bool aSyncHavok = true);
        virtual void sub_CB();
        virtual void Resurrect(bool aResetInventory);

        // Casting
        ActorExtension* GetExtension() noexcept;
        ExActor* AsExActor() noexcept;
        ExPlayerCharacter* AsExPlayerCharacter() noexcept;

        // Getters
        float GetSpeed() noexcept;
        TESForm* GetEquippedWeapon(uint32_t aSlotId) const noexcept;
        Inventory GetInventory() const noexcept;
        Factions GetFactions() const noexcept;
        ActorValues GetEssentialActorValues() noexcept;

        // Setters
        void SetSpeed(float aSpeed) noexcept;
        void SetLevelMod(uint32_t aLevel) noexcept;
        void SetInventory(const Inventory& acInventory) noexcept;
        void SetActorValues(const ActorValues& acActorValues) noexcept;
        void SetFactions(const Factions& acFactions) noexcept;
        void SetFactionRank(const TESFaction* acpFaction, int8_t aRank) noexcept;
        void ForcePosition(const NiPoint3& acPosition) noexcept;

        // Actions
        void UnEquipAll() noexcept;
        void QueueUpdate() noexcept;
        void RemoveFromAllFactions() noexcept;

        bool IsDead() noexcept;
        void Kill() noexcept;
        void Reset() noexcept;
        void Respawn() noexcept;

        uint32_t m_uiActorFlags;
        uint8_t unk2D8[0x300 - 0x2D8];
        AIProcess* m_pAIProcess;
        uint8_t pad308[0x3E8 - 0x308];
        TESForm* m_pMagicItems[4];
        uint8_t padActorEnd[0x490 - 0x408];
    };

    static_assert(sizeof(Actor) == 0x490);
}
