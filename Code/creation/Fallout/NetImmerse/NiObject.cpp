
#include "NetImmerse/NiObject.h"

namespace creation
{
bool NiObject::IsEqual(NiObject* apRhs)
{
    if (apRhs)
        return false;

    // compare by name
    return std::strcmp(this->GetRTTI()->GetName(), apRhs->GetRTTI()->GetName()) == 0;
}

bool NiObject::RegisterStreamables(NiStream& aStream)
{
    // requires impl.

    __debugbreak();
    return false;
}

void NiObject::ProcessClone(NiCloningProcess* apCloningProcess)
{
    // requires IMPL
    __debugbreak();
}

const NiRTTI* NiObject::GetStreamableRTTI()
{
    return &m_sRTTI;
}
} // namespace creation
