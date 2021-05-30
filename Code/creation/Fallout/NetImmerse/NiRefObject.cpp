
#include "NetImmerse/NiRefObject.h"

namespace creation
{
// atomic automatically uses intrinsics as the game does
// TODO: make this a pointer to the real game counter
// as a lot is inlined.
std::atomic<uint32_t> NiRefObject::ms_uiObjects = 0;

NiRefObject::NiRefObject()
{
    m_uiRefCount = 0;
    ms_uiObjects++;
}

NiRefObject::~NiRefObject()
{
    ms_uiObjects--;
}

void NiRefObject::DeleteThis()
{
    if (this)
        delete this;
}

void NiRefObject::IncRefCount()
{
    m_uiRefCount++;
}

void NiRefObject::DecRefCount()
{
    m_uiRefCount--;
    if (m_uiRefCount == 0)
        DeleteThis();
}
} // namespace creation
