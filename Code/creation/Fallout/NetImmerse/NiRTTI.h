#pragma once

#include <atomic>

namespace creation
{
    class NiRTTI
    {
    public:
        NiRTTI(const char* pcName, const NiRTTI* pkBaseRTTI) : m_pcName(pcName), m_pkBaseRTTI(pkBaseRTTI)
        {
        }

        inline const char* GetName() const
        {
            return m_pcName;
        }

        inline const NiRTTI* GetBaseRTTI() const
        {
            return m_pkBaseRTTI;
        }

    private:
        const char* m_pcName;
        const NiRTTI* m_pkBaseRTTI;
    };

    static_assert(sizeof(NiRTTI) == 16);
} // namespace creation
