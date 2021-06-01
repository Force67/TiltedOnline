#pragma once

#include "ActiveEffect.h"

namespace creation
{
    class Actor;
    class ActorValueInfo;

    class ValueModifierEffect : public ActiveEffect
    {
    public:
        virtual void sub_19();
        virtual void sub_1A();
        virtual void sub_1B();
        virtual void sub_1C();
        virtual void sub_1D();
        virtual void sub_1E();
        virtual void sub_1F();
        virtual void ApplyActorEffect(Actor* actor, float effectValue, unsigned int unk1);

        ActorValueInfo* m_pActorValueInfo; // When porting to skyrim, be aware that skyrim uses an index, not a ptr
        uint8_t padA0[0x8];
    };
    static_assert(sizeof(ValueModifierEffect) == 0xA8);
}
