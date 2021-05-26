#pragma once

#include <stdafx.h>

#include "Shared/TESForms/FormComponents/BaseFormComponent.h"

namespace creation
{
    // TODO: move to inl
    enum FormType : int
    {
        kGlobal = 11,
        kYear = 53,
        kMonth = 54,
        kDay = 55,
        kHour = 56,
        kDayCounter = 57,
        kTimeScale = 58
    };

    class TESForm : public BaseFormComponent
    {
    public:
        void InitializeDataComponent() override {}
        void ClearDataComponent() override {}
        void CopyComponent(BaseFormComponent *apBase) {} // TODO: impl

        template<typename T>
        static T* GetFormById(FormType aType)
        {
            return reinterpret_cast<T*>(GetFormByIdImpl(aType));
        }

    private:
        static TESForm* GetFormByIdImpl(FormType aType);

    protected:
        const void* m_pFileList;
        uint32_t m_uiFlags;
        uint32_t m_uiId;
        uint16_t m_uiFlags2;
        uint16_t m_uiType;
    };

    static_assert(sizeof(TESForm) == 0x20);
}
