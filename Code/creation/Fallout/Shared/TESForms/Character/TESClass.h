#pragma once

#include "Shared/FormComponents/TESForm.h"
#include "Shared/FormComponents/TESFullName.h"
#include "Shared/FormComponents/TESDescription.h"
#include "Shared/FormComponents/TESTexture.h"
#include "Shared/FormComponents/BGSPropertySheet.h"

namespace creation
{
    class TESClass : public TESForm, public TESFullName, public TESDescription,
                     public TESTexture, public BGSPropertySheet
    {
        virtual ~TESClass();

        uint8_t unk68[0x8];
    };

    static_assert(sizeof(TESClass) == 0x70);
}
