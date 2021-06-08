#pragma once

#include "BaseFormComponent.h"

namespace creation
{
    class TESRace;

    class TESRaceForm : public BaseFormComponent
    {
        virtual ~TESRaceForm();

        TESRace* race;
    };
}
