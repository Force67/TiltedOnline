#pragma once

class ActorValueInfo;

namespace creation
{
    class ActorValueOwner
    {
        enum ValueMode : uint32_t
        {
            kPermanent,
            kTemporary,
            kDamage,
            kCount
        };

    public:
        virtual ~ActorValueOwner();
        virtual float GetValue(ActorValueInfo* apInfo) const noexcept;
        virtual float GetPermanentValue(ActorValueInfo* apInfo) const noexcept;
        virtual float GetBaseValue(ActorValueInfo* apInfo) const noexcept;
        virtual void SetBaseValue(ActorValueInfo* apInfo, float aValue) noexcept;
        virtual void ModBaseValue(ActorValueInfo* apInfo, float aValue) noexcept;
        virtual void ModValue(ValueMode aMode, ActorValueInfo* apInfo, float aValue) noexcept;
        virtual float GetModifier(ValueMode aMode, ActorValueInfo* apInfo) const noexcept;
        virtual void RestoreValue(ActorValueInfo* apInfo, float aAmount) noexcept;
        virtual void SetValue(ActorValueInfo* apInfo, float aValue) noexcept;
        virtual bool IsPlayerOwner() const noexcept;
    };

    static_assert(sizeof(ActorValueOwner) == 0x8);
}
