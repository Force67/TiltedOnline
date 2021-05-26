#pragma once

#include <atomic>

namespace creation
{
    class NiRTTI
    {
    public:
        NiRTTI(const char* pName, const NiRTTI* pBaseRttiInfo) : m_pName(pName), m_pBaseRTTI(pBaseRttiInfo)
        {
        }

        const char* GetName() const
        {
            return m_pName;
        }

        const NiRTTI* GetBaseRTTI() const
        {
            return m_pBaseRTTI;
        }

    private:
        const char* m_pName;
        const NiRTTI* m_pBaseRTTI;
    };

    static_assert(sizeof(NiRTTI) == 16);
} // namespace creation
