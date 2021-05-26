
#include "Shared/Calendar/Calendar.h"

namespace creation
{
    namespace
    {
        const int kDayLengthArray[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        int GetNumerOfDaysByMonthIndex(int aIndex)
        {
            if (aIndex < 12)
            {
                return kDayLengthArray[aIndex];
            }

            return 0;
        }
    }

    Calendar::Calendar()
    {
        // TODO: set globals

    }

    Calendar* Calendar::GetInstance()
    {
        return s_mpInstance;
    }

    void Calendar::Update(float aTick)
    {
        float deltaSeconds = aTick / 1000.f;
        float time = m_pGameHour->GetValue() + (deltaSeconds * (m_pTimeScale->GetValue() * 0.00027777778f));
        float day = m_pGameDay->GetValue();
        float month = m_pGameMonth->GetValue();

        if (time > 24.f)
        {
            float maxDays = static_cast<float>(GetNumerOfDaysByMonthIndex(
                static_cast<int>(m_pGameMonth->GetValue())));

            do 
            {
                time = time + -24.f;
                day++;
            } while (time > 24.f);

            if (day > maxDays)
            {
                month++;
                day = day - maxDays;

                if (month >= 13.f)
                {
                    month = month + -12.f;
                    m_pGameYear->value++;
                }

                m_pGameMonth->SetValue(month);
            }

            m_pGameDay->SetValue(day);
            ++m_uiNightCounter;
            m_fDayCounter++;
        }

        m_pGameDaysPassed->SetValue((time * 0.041666668) + m_fDayCounter);
        m_pGameHour->SetValue(time);
    }
}
