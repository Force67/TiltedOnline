#include "TESForm.h"

namespace creation
{
    TESForm* TESForm::GetFormByIdImpl(FormType aType)
    {
        return 0;
    }

    void TESForm::Save_Reversed(const uint32_t aChangeFlags, Buffer::Writer& aWriter)
    {
        if(aChangeFlags & 1)
        {
            aWriter.WriteBytes(reinterpret_cast<uint8_t*>(&m_uiFlags), 4);
            aWriter.WriteBytes(reinterpret_cast<uint8_t*>(&m_uiFlags2), 2);
        }
    }

    void TESForm::SetSkipSaveFlag(bool aSet) noexcept
    {
        if(aSet)
        {
            m_uiFlags2 = 0xFFFF;
        }
        /*const uint32_t flag = 1 << 14;

        if (aSet)
            flags |= flag;
        else
            flags &= ~flag;*/
    }

    uint32_t TESForm::GetChangeFlags() const noexcept
    {
        struct Unk
        {
            uint8_t unk0[0x350];
            void* unk330;
        };

        TP_THIS_FUNCTION(InternalGetChangeFlags, bool, void, uint32_t formId, ChangeFlags& changeFlags);

        POINTER_FALLOUT4(InternalGetChangeFlags, internalGetChangeFlags, 0x140CD8160 - 0x140000000);
        POINTER_SKYRIMSE(InternalGetChangeFlags, internalGetChangeFlags, 0x140577EF0 - 0x140000000);

        POINTER_FALLOUT4(Unk*, s_singleton, 0x145A73360 - 0x140000000);
        POINTER_SKYRIMSE(Unk*, s_singleton, 0x142F266F8 - 0x140000000);

        const auto pUnk = *(s_singleton.Get());
        
        ChangeFlags changeFlags;
        const auto cResult = ThisCall(internalGetChangeFlags, pUnk->unk330, m_uiId, changeFlags);
        if (!cResult)
            return 0;

        return changeFlags.flags;
    }
}
