#pragma once

#include <stdafx.h>

#include "BSCore/BSTArray.h"
#include "BSSystem/BSTSmartPointer.h"
#include "NavMesh.h"

namespace creation
{
    class NavMeshArray
    {
    private:
        BSTArray<BSTSmartPointer<NavMesh>> m_NavMeshArray;
    };
}
