#pragma once

#include "BaseFormComponent.h"
#include "IKeywordFormBase.h"

namespace creation
{
    class BGSKeyword;

    class BGSKeywordForm : public BaseFormComponent, public IKeywordFormBase
    {
        virtual ~BGSKeywordForm();

        BGSKeyword** m_pKeywords; // 10
        uint32_t m_uiCount; // 18
        uint32_t pad;
    };

    static_assert(sizeof(BGSKeywordForm) == 0x20);
}
