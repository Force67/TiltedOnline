#pragma once

#include "Shared/FormComponents/TESForm.h"
#include "BSCore/BSTArray.h"

namespace creation
{
    class BGSOutfit : public TESForm
    {
        BSTArray<TESForm*> m_outfitItems;
    };

    static_assert(sizeof(BGSOutfit) == 0x38);
}
