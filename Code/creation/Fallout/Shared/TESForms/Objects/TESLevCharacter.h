#pragma once

#include "TESBoundAnimObject.h"
#include "Shared/FormComponents/TESLeveledList.h"
#include "Shared/FormComponents/BGSModelMaterialSwap.h"

namespace creation
{
    class TESLevCharacter : public TESBoundAnimObject, public TESLeveledList,
                            public BGSModelMaterialSwap
    {
    };

    static_assert(sizeof(TESLevCharacter) == 0xD8);
}
