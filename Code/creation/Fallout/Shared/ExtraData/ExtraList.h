#pragma once

#include "BSExtraData.h"

namespace creation
{
    class ExtraList
    {
        BSExtraData *m_pBegin;
        BSExtraData **m_pEnd;
        uint8_t *m_pFlags;
    };
}
