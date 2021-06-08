#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class TESForm;

    class BGSSkinForm : public BaseFormComponent
    {
        virtual ~BGSSkinForm();

        TESForm* m_pForm; // this is wrong
    };

    static_assert(sizeof(BGSSkinForm) == 0x10);
}
