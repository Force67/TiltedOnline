#include <stdafx.h>

#include "TESObjectREFR.h"
#include "Shared/AI/Actor.h"

TP_THIS_FUNCTION(TRotate, void, TESObjectREFR, float aAngle);

static TRotate* RealRotateX = nullptr;
static TRotate* RealRotateY = nullptr;
static TRotate* RealRotateZ = nullptr;

namespace creation
{
    void TESObjectREFR::SetRotation(float aX, float aY, float aZ) noexcept
    {
        ThisCall(RealRotateX, this, aX);
        ThisCall(RealRotateY, this, aY);
        ThisCall(RealRotateZ, this, aZ);
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
            BSScopedLock<BSRecursiveLock> _{pManager->lock};

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

    TiltedPhoques::Initializer s_referencesHooks([]()
        {
            POINTER_FALLOUT4(TSetPosition, s_setPosition, 0x14040C060 - 0x140000000);
            POINTER_FALLOUT4(TRotate, s_rotateX, 0x14040BE70 - 0x140000000);
            POINTER_FALLOUT4(TRotate, s_rotateY, 0x14040BF00 - 0x140000000);
            POINTER_FALLOUT4(TRotate, s_rotateZ, 0x14040BF90 - 0x140000000);
            POINTER_FALLOUT4(TActorProcess, s_actorProcess, 0x140D7CEB0 - 0x140000000);
            POINTER_FALLOUT4(TActivate, s_activate, 0x14040C750 - 0x140000000);
            POINTER_FALLOUT4(TLockChange, s_lockChange, 0x1403EDBA0 - 0x140000000);

            RealSetPosition = s_setPosition.Get();
            RealRotateX = s_rotateX.Get();
            RealRotateY = s_rotateY.Get();
            RealRotateZ = s_rotateZ.Get();
            RealActorProcess = s_actorProcess.Get();
            RealActivate = s_activate.Get();
            RealLockChange = s_lockChange.Get();

            TP_HOOK(&RealSetPosition, HookSetPosition);
            TP_HOOK(&RealRotateX, HookRotateX);
            TP_HOOK(&RealRotateY, HookRotateY);
            TP_HOOK(&RealRotateZ, HookRotateZ);
            TP_HOOK(&RealActorProcess, HookActorProcess);
            TP_HOOK(&RealActivate, HookActivate);
            TP_HOOK(&RealLockChange, HookLockChange);
        });
}
