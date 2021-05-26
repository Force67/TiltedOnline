#pragma once

#include <cstdint>
#include "Shared/TESForms/FormComponents/BaseFormComponent.h"

namespace creation
{
    // TODO: move to inl
    enum FormType
    {
        kGlobal = 11,
    };

    class TESForm : public BaseFormComponent
    {
    public:
        virtual void InitializeDataComponent() override {}
        virtual void ClearDataComponent() override {}
        virtual void CopyComponent(BaseFormComponent *apBase) {} // TODO: impl

    protected:
        const void* m_pFileList;
        uint32_t m_uiFlags;
        uint32_t m_uiId;
        uint16_t m_uiFlags2;
        uint8_t m_uiType;
    };

    static_assert(sizeof(TESForm) == 0x20);
}
