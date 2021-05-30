#pragma once

#include "Shared/TESForms/Gameplay/TESGlobal.h"

namespace creation
{
    class Calendar
    {
    public:
        Calendar();

        static Calendar* GetInstance();

        void Update(float aTick);

    private:
        static Calendar* s_mpInstance;

        uint32_t pad0;
        TESGlobal* m_pGameYear;
        TESGlobal* m_pGameMonth;
        TESGlobal* m_pGameDay;
        TESGlobal* m_pGameHour;
        TESGlobal* m_pGameDaysPassed;
        TESGlobal* m_pTimeScale;
        uint32_t m_uiNightCounter = 0;
        float m_fDayCounter = 0.f;
    };

    static_assert(sizeof(Calendar) == 0x40);
}
