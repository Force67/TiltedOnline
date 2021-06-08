#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class ActorValueInfo;

    class BGSForcedLocRefType : public BaseFormComponent
    {
        virtual ~BGSForcedLocRefType();

        ActorValueInfo** m_pValues; // this is wrong
    };
}
