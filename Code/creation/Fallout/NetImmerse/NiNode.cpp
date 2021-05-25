
#include "NetImmerse/NiNode.h"

namespace creation
{
    NiRTTI* NiNode::GetRTTI() const
    {
        return &m_sRTTI;
    }

    NiNode* NiNode::IsNode()
    {
        return this;
    }

    NiNode* NiNode::IsNode2()
    {
        return this;
    }


}
