#pragma once

#include "BSExtraData.h"

namespace creation
{
    class TESNPC;
    class TESActorBase;

    class ExtraLeveledCreature : public BSExtraData
    {
    public:
        virtual ~ExtraLeveledCreature();

        TESActorBase* m_pBase;
        TESActorBase* m_pTemplates[13];
    };

    static_assert(sizeof(ExtraLeveledCreature) == 0x88);
    static_assert(offsetof(ExtraLeveledCreature, m_pBase) == 0x18);
}
