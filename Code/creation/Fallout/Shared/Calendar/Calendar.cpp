
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
        m_pGameYear = TESForm::GetFormById<TESGlobal>(FormType::kYear);
        m_pGameMonth = TESForm::GetFormById<TESGlobal>(FormType::kMonth);
        m_pGameDay = TESForm::GetFormById<TESGlobal>(FormType::kDay);
        m_pGameHour = TESForm::GetFormById<TESGlobal>(FormType::kHour);
        m_pGameDaysPassed = TESForm::GetFormById<TESGlobal>(FormType::kDayCounter);
        m_pTimeScale = TESForm::GetFormById<TESGlobal>(FormType::kTimeScale);

        m_pGameDaysPassed->value = (m_pGameHour->value * 0.041666668) + m_pGameDaysPassed->value;
    }

     // proves that the compiler divides by 12
    // constexpr float y = 0.041666668 * 24;
    // value / 12 * 0.5

    // constexpr float z = 0.00027777778f * 60 / 1000;
    // proves a division by 1/60

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
