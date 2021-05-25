#pragma once

#include "BSCore/BSFixedString.h"

#include "NetImmerse/NiObject.h"
#include "NetImmerse/NiStream.h"

namespace creation
{
    class NiObjectNET : public NiObject
    {
    public:
        NiObjectNET() = default;
        virtual ~NiObjectNET() = default;

        virtual NiRTTI* GetRTTI() const override;

        void LoadBinary(NiStream&) override;
        void LinkObject(NiStream&) override;
        bool RegisterStreamables(NiStream&) override;
        void SaveBinary(NiStream&) override;
        bool IsEqual(NiObject* apRhs) override;
        void ProcessClone(NiCloningProcess* apCloningProcess) override;
        void PostLinkObject(NiStream&) override;

    private:
        BSFixedString m_Name;
        void* pUnk1;
        void* pUnk2;

        static NiRTTI m_sRTTI;
    };

    static_assert(sizeof(NiObjectNET) == 0x28);
} // namespace creation
