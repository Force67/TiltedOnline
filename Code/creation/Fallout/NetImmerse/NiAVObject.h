#pragma once

#include "NetImmerse/NiObjectNET.h"
#include "NetImmerse/NiBound.h"
#include "NetImmerse/NiTransform.h"
#include "NetImmerse/NiPointer.h"

namespace creation
{
    class NiCollisionObject;

    class NiAVObject : public NiObjectNET
    {
    public:
        virtual ~NiAVObject() = default;

    private:
        NiNode* m_pkParent;
        NiTransform m_kLocal;
        NiTransform m_kWorld;
        NiBound m_kWorldBound;
        NiTransform m_kPreviousWorld;
        NiPointer<NiCollisionObject> m_pCollideObject;
        uint64_t m_Flags;
        uint64_t m_unk5;
        float fFadeAmount;
        uint8_t m_type;
        uint8_t m_lodFadingLevel;
        uint8_t m_lodRelated1;
        uint8_t m_lodRelated2;
    };

    static_assert(sizeof(NiAVObject) == 0x120);
}
