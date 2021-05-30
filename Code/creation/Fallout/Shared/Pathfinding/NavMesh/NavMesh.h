#pragma once

#include <stdafx.h>

#include "Shared/FormComponents/TESForm.h"
#include "BSNavmesh.h"

namespace creation
{
    class NavMesh : public TESForm, public BSNavmesh
    {
    };
}
