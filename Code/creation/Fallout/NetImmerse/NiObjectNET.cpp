
#include "NetImmerse/NiObjectNET.h"

namespace creation
{
    NiRTTI* NiObjectNET::GetRTTI() const
    {
        return &m_sRTTI;
    }

    void NiObjectNET::LoadBinary(NiStream&)
    {
    }

    void NiObjectNET::LinkObject(NiStream&)
    {
    }

    bool NiObjectNET::RegisterStreamables(NiStream&)
    {
        return false;
    }

    void NiObjectNET::SaveBinary(NiStream&)
    {
    }

    bool NiObjectNET::IsEqual(NiObject* apRhs)
    {
        return false;
    }

    void NiObjectNET::ProcessClone(NiCloningProcess* apCloningProcess)
    {
    }

    void NiObjectNET::PostLinkObject(NiStream&)
    {
    }

} // namespace creation
