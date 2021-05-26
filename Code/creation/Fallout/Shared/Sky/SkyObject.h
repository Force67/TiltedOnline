#pragma once

#include "NetImmerse/NiNode.h"
#include "NetImmerse/NiPointer.h"

namespace creation
{
    class Sky;

    class SkyObject
    {
    public:
        SkyObject() = default;
        virtual ~SkyObject() = default;

        // Moved to non virtual from Skyrim
        void Init(NiNode* apRoot);

        virtual NiNode* GetRoot();
        virtual void Update(Sky* apSky, float aTick) {};
    private:
        NiPointer<NiNode> m_pRootNode;
    };

    static_assert(sizeof(SkyObject) == 16);
} // namespace creation
