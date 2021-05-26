#pragma once

#include <stdafx.h>

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
        void InitializeDataComponent() override {}
        void ClearDataComponent() override {}
        void CopyComponent(BaseFormComponent *apBase) {} // TODO: impl

    protected:
        const void* m_pFileList;
        uint32_t m_uiFlags;
        uint32_t m_uiId;
        uint16_t m_uiFlags2;
        uint16_t m_uiType;
    };

    static_assert(sizeof(TESForm) == 0x20);
}
