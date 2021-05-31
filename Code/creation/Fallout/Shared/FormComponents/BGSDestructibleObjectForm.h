#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class TESForm;

    class BGSDestructibleObjectForm : public BaseFormComponent
    {
    public:
        virtual ~BGSDestructibleObjectForm();

        TESForm* m_pForm; // this is wrong
    };

    static_assert(sizeof(BGSDestructibleObjectForm) == 0x10);
}
