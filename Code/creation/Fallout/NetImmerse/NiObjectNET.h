#pragma once

#include "BSCore/BSFixedString.h"
#include "NetImmerse/NiObject.h"

namespace creation
{
class NiObjectNET : public NiObject
{
  public:
    NiObjectNET() = default;
    virtual ~NiObjectNET() = default;

    virtual NiRTTI* GetRTTI() override
    {
        return &m_sRTTI;
    }

  private:
    BSFixedString m_Name;
    void* pUnk1;
    void* pUnk2;

    static NiRTTI m_sRTTI;
};

static_assert(sizeof(NiObjectNET) == 0x28);
} // namespace creation
