#pragma once

#include "BaseFormComponent.h"
#include "BSCore/BSFixedString.h"

namespace creation
{
    class TESTexture : public BaseFormComponent
    {
    protected:
        BSFixedString m_textureName;
    };

    static_assert(sizeof(TESTexture) == 0x10);
}

