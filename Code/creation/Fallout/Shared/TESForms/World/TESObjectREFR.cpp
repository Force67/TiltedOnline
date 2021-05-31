#include "TESObjectREFR.h"
#include "Shared/AI/Actor.h"
#include "BSCore/BSScopedLock.h"
#include "BSCore/BSSpinLock.h"

#include "Game/SaveGame/Buffers/BGSSaveFormBuffer.h"
#include "Game/SaveGame/Buffers/BGSLoadFormBuffer.h"

#include "BSAnimation/BSAnimationGraphManager.h"
#include "Shared/Havok/hkbBehaviorGraph.h"
#include "Shared/Havok/StateMachine/hkbStateMachine.h"

#include "Shared/ExtraData/Lock.h"

#include <TiltedCore/Serialization.hpp>
#include <Structs/AnimationGraphDescriptorManager.h>
#include <Structs/AnimationVariables.h>
#include <Overrides.h>

using ScopedReferencesOverride = ScopedOverride<TESObjectREFR>;
thread_local uint32_t ScopedReferencesOverride::s_refCount = 0;

TP_THIS_FUNCTION(TRotate, void, TESObjectREFR, float aAngle);
TP_THIS_FUNCTION(TActivate, void, TESObjectREFR, TESObjectREFR* apActivator, uint8_t aUnk1, int64_t aUnk2, int aUnk3, char aUnk4);
TP_THIS_FUNCTION(TLockChange, void, TESObjectREFR);

static TRotate* RealRotateX = nullptr;
static TRotate* RealRotateY = nullptr;
static TRotate* RealRotateZ = nullptr;
static TActivate* RealActivate = nullptr;
static TLockChange* RealLockChange = nullptr;

namespace creation
{
    void TESObjectREFR::SetRotation(float aX, float aY, float aZ) noexcept
    {
        ThisCall(RealRotateX, this, aX);
        ThisCall(RealRotateY, this, aY);
        ThisCall(RealRotateZ, this, aZ);
    }

    uint32_t TESObjectREFR::GetCellId() const noexcept
    {
        if (!m_pParentCell)
            return 0;

        const auto* pWorldSpace = m_pParentCell->m_pWorldSpace;

        return pWorldSpace != nullptr ? pWorldSpace->m_uiId : m_pParentCell->m_uiId;
    }

    TESWorldSpace* TESObjectREFR::GetWorldSpace() const noexcept
    {
        auto* pParentCell = m_pParentCell ? m_pParentCell : GetParentCell();
        if (pParentCell && !(pParentCell->m_uiCellFlags & 1))
            return pParentCell->m_pWorldSpace;

        return nullptr;
    }

    BSTSmartPointer<ExtraDataList> TESObjectREFR::GetExtraDataList() noexcept
    {
        return m_pExtraData;
    }

    ActorValueInfo* TESObjectREFR::GetActorValueInfo(uint32_t aId) noexcept
    {
        using TGetActorValueInfoArray = ActorValueInfo**();

        POINTER_FALLOUT4(TGetActorValueInfoArray, s_getActorValueInfoArray, 0x14006B1F0 - 0x140000000);

        ActorValueInfo** actorValueInfoArray = s_getActorValueInfoArray.Get()();

        return actorValueInfoArray[aId];
    }

    Lock* TESObjectREFR::GetLock() noexcept
    {
        TP_THIS_FUNCTION(TGetLock, Lock*, TESObjectREFR);
        POINTER_FALLOUT4(TGetLock, realGetLock, 0x14047FEE0 - 0x140000000);

        return ThisCall(realGetLock, this);
    }

    using TiltedPhoques::Serialization;

    void TESObjectREFR::SaveAnimationVariables(AnimationVariables& aVariables) const noexcept
    {
        BSTSmartPointer<BSAnimationGraphManager> pManager;
        if (GetBSAnimationGraph(&pManager))
        {
            BSScopedLock<BSSpinLock> _{pManager->lock};

            if (pManager->animationGraphIndex < pManager->animationGraphs.size)
            {
                const auto* pGraph = pManager->animationGraphs.Get(pManager->animationGraphIndex);

                if (!pGraph)
                    return;
            
                if (!pGraph->behaviorGraph || !pGraph->behaviorGraph->stateMachine ||
                    !pGraph->behaviorGraph->stateMachine->name)
                    return;

                const auto* pDescriptor =
                    AnimationGraphDescriptorManager::Get().GetDescriptor(pGraph->behaviorGraph->stateMachine->name);

                if (!pDescriptor)
                    return;

                const auto* pVariableSet = pGraph->behaviorGraph->animationVariables;

                if (!pVariableSet)
                    return;
                
                aVariables.Booleans = 0;

                aVariables.Floats.resize(pDescriptor->FloatLookupTable.size());
                aVariables.Integers.resize(pDescriptor->IntegerLookupTable.size());

                for (size_t i = 0; i < pDescriptor->BooleanLookUpTable.size(); ++i)
                {
                    const auto idx = pDescriptor->BooleanLookUpTable[i];

                    if (pVariableSet->data[idx] != 0)
                        aVariables.Booleans |= (1ull << i);
                }

                for (size_t i = 0; i < pDescriptor->FloatLookupTable.size(); ++i)
                {
                    const auto idx = pDescriptor->FloatLookupTable[i];
                    aVariables.Floats[i] = *reinterpret_cast<float*>(&pVariableSet->data[idx]);
                }

                for (size_t i = 0; i < pDescriptor->IntegerLookupTable.size(); ++i)
                {
                    const auto idx = pDescriptor->IntegerLookupTable[i];
                    aVariables.Integers[i] = *reinterpret_cast<uint32_t*>(&pVariableSet->data[idx]);
                }
            }

            pManager->Release();
        }
    }

    void TESObjectREFR::SaveInventory(BGSSaveFormBuffer* apBuffer) const noexcept
    {
        TP_THIS_FUNCTION(TSaveFunc, void, void, BGSSaveFormBuffer*);

        POINTER_FALLOUT4(TSaveFunc, s_save, 0x1401ACB20 - 0x140000000);

        ThisCall(s_save, m_pInventory, apBuffer);
    }

    String TESObjectREFR::SerializeInventory() const noexcept
    {
        ScopedSaveLoadOverride _;

        char buffer[1 << 15];
        BGSSaveFormBuffer saveBuffer;
        saveBuffer.m_pBuffer = buffer;
        saveBuffer.m_uiCapacity = 1 << 15;
        saveBuffer.m_uiChangeFlags = 1024;

        SaveInventory(&saveBuffer);

        return String(buffer, saveBuffer.m_uiPosition);
    }

    void TESObjectREFR::LoadAnimationVariables(const AnimationVariables& aVariables) const noexcept
    {
        BSTSmartPointer<BSAnimationGraphManager> pManager;
        if (GetBSAnimationGraph(&pManager))
        {
            BSScopedLock<BSSpinLock> _{pManager->lock};

            if (pManager->animationGraphIndex < pManager->animationGraphs.size)
            {
                const auto* pGraph = pManager->animationGraphs.Get(pManager->animationGraphIndex);

                if (!pGraph)
                    return;
                
                if (!pGraph->behaviorGraph || !pGraph->behaviorGraph->stateMachine ||
                    !pGraph->behaviorGraph->stateMachine->name)
                    return;

                const auto* pDescriptor =
                    AnimationGraphDescriptorManager::Get().GetDescriptor(pGraph->behaviorGraph->stateMachine->name);

                if (!pDescriptor)
                {
                    //if ((formID & 0xFF000000) == 0xFF000000)
                        //spdlog::info("Form id {} has {}", formID, pGraph->behaviorGraph->stateMachine->name);
                    return;
                }

                const auto* pVariableSet = pGraph->behaviorGraph->animationVariables;
                
                if (!pVariableSet)
                    return;
                
                for (size_t i = 0; i < pDescriptor->BooleanLookUpTable.size(); ++i)
                {
                    const auto idx = pDescriptor->BooleanLookUpTable[i];

                    if (pVariableSet->size > idx)
                    {
                        pVariableSet->data[idx] = (aVariables.Booleans & (1ull << i)) != 0;
                    }
                }

                for (size_t i = 0; i < pDescriptor->FloatLookupTable.size(); ++i)
                {
                    const auto idx = pDescriptor->FloatLookupTable[i];
                    *reinterpret_cast<float*>(&pVariableSet->data[idx]) = aVariables.Floats.size() > i ? aVariables.Floats[i] : 0.f;
                }

                for (size_t i = 0; i < pDescriptor->IntegerLookupTable.size(); ++i)
                {
                    const auto idx = pDescriptor->IntegerLookupTable[i];
                    *reinterpret_cast<uint32_t*>(&pVariableSet->data[idx]) = aVariables.Integers.size() > i ? aVariables.Integers[i] : 0;
                }
            }

            pManager->Release();
        }
    }

    void TESObjectREFR::LoadInventory(BGSLoadFormBuffer* apBuffer) noexcept
    {
        TP_THIS_FUNCTION(TLoadFunc, void, void, BGSLoadFormBuffer*);

        POINTER_FALLOUT4(TLoadFunc, s_load, 0x1401ACC00 - 0x140000000);

        sub_A8();
        sub_A7(nullptr);

        ThisCall(s_load, m_pInventory, apBuffer);
    }

    void TESObjectREFR::DeserializeInventory(const String& acData) noexcept
    {
        ScopedSaveLoadOverride _;

        BGSLoadFormBuffer loadBuffer(1024);
        loadBuffer.SetSize(acData.size() & 0xFFFFFFFF);
        loadBuffer.buffer = acData.c_str();
        loadBuffer.formId = 0;
        loadBuffer.form = nullptr;
        
        LoadInventory(&loadBuffer);
    }

    void TESObjectREFR::RemoveAllItems() noexcept
    {
        using TRemoveAllItems = void(void*, void*, TESObjectREFR*, TESObjectREFR*, bool);

        POINTER_FALLOUT4(TRemoveAllItems, s_removeAllItems, 0x14140CE90 - 0x140000000);

        s_removeAllItems(nullptr, nullptr, this, nullptr, false);
    }

    void TESObjectREFR::Delete() const noexcept
    {
        using ObjectReference = TESObjectREFR;

        PAPYRUS_FUNCTION(void, ObjectReference, Delete);

        s_pDelete(this);
    }

    void TESObjectREFR::Disable() const noexcept
    {
        using ObjectReference = TESObjectREFR;

        PAPYRUS_FUNCTION(void, ObjectReference, Disable, bool);

        s_pDisable(this, true);
    }

    void TESObjectREFR::Enable() const noexcept
    {
        using ObjectReference = TESObjectREFR;

        PAPYRUS_FUNCTION(void, ObjectReference, Enable, bool);

        s_pEnable(this, true);
    }

    void TESObjectREFR::MoveTo(TESObjectCELL* apCell, const NiPoint3& acPosition) const noexcept
    {
        ScopedReferencesOverride recursionGuard;

        TP_THIS_FUNCTION(TInternalMoveTo, bool, const TESObjectREFR, uint32_t*&, TESObjectCELL*, TESWorldSpace*,
                         const NiPoint3&, const NiPoint3&);

        POINTER_SKYRIMSE(TInternalMoveTo, s_internalMoveTo, 0x1409AE5C0 - 0x140000000);
        POINTER_FALLOUT4(TInternalMoveTo, s_internalMoveTo, 0x1413FE7E0 - 0x140000000);

        ThisCall(s_internalMoveTo, this, s_nullHandle.Get(), apCell, apCell->m_pWorldSpace, acPosition, m_rotation);
    }

    void TESObjectREFR::Activate(TESObjectREFR* apActivator, uint8_t aUnk1, int64_t aUnk2, int aUnk3, char aUnk4) noexcept
    {
        return ThisCall(RealActivate, this, apActivator, aUnk1, aUnk2, aUnk3, aUnk4);
    }

    Lock* TESObjectREFR::CreateLock() noexcept
    {
        TP_THIS_FUNCTION(TCreateLock, Lock*, TESObjectREFR);
        POINTER_FALLOUT4(TCreateLock, realCreateLock, 0x14047FD20 - 0x140000000);

        return ThisCall(realCreateLock, this);
    }

    void TESObjectREFR::LockChange() noexcept
    {
        ThisCall(RealLockChange, this);
    }

    void TP_MAKE_THISCALL(HookRotateX, TESObjectREFR, float aAngle)
    {
        if(apThis->m_uiType == Actor::Type)
        {
            const auto pActor = static_cast<Actor*>(apThis);
            // We don't allow remotes to move
            if (pActor->GetExtension()->IsRemote())
                return;
        }

        return ThisCall(RealRotateX, apThis, aAngle);
    }

    void TP_MAKE_THISCALL(HookRotateY, TESObjectREFR, float aAngle)
    {
        if (apThis->m_uiType == Actor::Type)
        {
            const auto pActor = static_cast<Actor*>(apThis);
            // We don't allow remotes to move
            if (pActor->GetExtension()->IsRemote())
                return;
        }

        return ThisCall(RealRotateY, apThis, aAngle);
    }

    void TP_MAKE_THISCALL(HookRotateZ, TESObjectREFR, float aAngle)
    {
        if (apThis->m_uiType == Actor::Type)
        {
            const auto pActor = static_cast<Actor*>(apThis);
            // We don't allow remotes to move
            if (pActor->GetExtension()->IsRemote())
                return;
        }

        return ThisCall(RealRotateZ, apThis, aAngle);
    }

    void TP_MAKE_THISCALL(HookActivate, TESObjectREFR, TESObjectREFR* apActivator, uint8_t aUnk1, int64_t aUnk2, int aUnk3, char aUnk4)
    {
        auto* pActivator = RTTI_CAST(apActivator, TESObjectREFR, Actor);
        if (pActivator)
            World::Get().GetRunner().Trigger(ActivateEvent(apThis, pActivator, aUnk1, aUnk2, aUnk3, aUnk4));

        return ThisCall(RealActivate, apThis, apActivator, aUnk1, aUnk2, aUnk3, aUnk4);
    }

    void TP_MAKE_THISCALL(HookLockChange, TESObjectREFR)
    {
        const auto* pLock = apThis->GetLock();
        uint8_t lockLevel = pLock->m_uiLockLevel;

        World::Get().GetRunner().Trigger(LockChangeEvent(apThis, pLock->m_uiFlags, lockLevel));

        ThisCall(RealLockChange, apThis);
    }

    TiltedPhoques::Initializer s_referencesHooks([]()
        {
            POINTER_FALLOUT4(TRotate, s_rotateX, 0x14040BE70 - 0x140000000);
            POINTER_FALLOUT4(TRotate, s_rotateY, 0x14040BF00 - 0x140000000);
            POINTER_FALLOUT4(TRotate, s_rotateZ, 0x14040BF90 - 0x140000000);
            POINTER_FALLOUT4(TActivate, s_activate, 0x14040C750 - 0x140000000);
            POINTER_FALLOUT4(TLockChange, s_lockChange, 0x1403EDBA0 - 0x140000000);

            RealRotateX = s_rotateX.Get();
            RealRotateY = s_rotateY.Get();
            RealRotateZ = s_rotateZ.Get();
            RealActivate = s_activate.Get();
            RealLockChange = s_lockChange.Get();

            TP_HOOK(&RealRotateX, HookRotateX);
            TP_HOOK(&RealRotateY, HookRotateY);
            TP_HOOK(&RealRotateZ, HookRotateZ);
            TP_HOOK(&RealActivate, HookActivate);
            TP_HOOK(&RealLockChange, HookLockChange);
        });
}
