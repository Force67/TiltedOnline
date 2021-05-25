#pragma once

#include "NetImmerse/NiObjectNET.h"

namespace creation
{
    class NiNode : public NiObjectNET
    {
    public:
        NiNode() = default;
        virtual ~NiNode() = default;

        virtual NiRTTI* GetRTTI() const override;

        NiNode* IsNode() override;
        NiNode* IsNode2() override;

        // TODO: override/impl remaining members.
        // TODO: add remaining member vars.

    private:
        static NiRTTI m_sRTTI;
    };

    static_assert(sizeof(NiObjectNET) == 0x28);
} // namespace creation
