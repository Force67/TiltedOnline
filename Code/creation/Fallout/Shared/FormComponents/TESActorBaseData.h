#pragma once

#include "BaseFormComponent.h"
#include "BSCore/BSTArray.h"

namespace creation
{
    class BGSVoiceType;

    class TESActorBaseData : public BaseFormComponent
    {
    public:
        virtual ~TESActorBaseData();
        virtual void sub_7();
        virtual void sub_8();
        virtual void sub_9();

        struct alignas(sizeof(void*)) FactionInfo
        {
            struct TESFaction* faction;
            int8_t rank;
        };

        uint64_t m_uiFlags;
        uint8_t unk10[0x28 - 0x10];
        BGSVoiceType* m_pVoiceType;
        TESForm* m_pOwner;
        uint8_t unk30[0x50 - 0x38];
        BSTArray<FactionInfo> m_factions;
    };

    static_assert(sizeof(TESActorBaseData) == 0x68);
    static_assert(offsetof(TESActorBaseData, m_pOwner) == 0x30);
    static_assert(offsetof(TESActorBaseData, m_factions) == 0x50);
}
