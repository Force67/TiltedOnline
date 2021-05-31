#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class BGSOverridePackCollection : public BaseFormComponent
    {
        virtual ~BGSOverridePackCollection();

        void* m_pPacks[6];
    };
}

