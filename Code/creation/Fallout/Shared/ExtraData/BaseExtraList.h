#pragma once

#include "BSExtraData.h"

namespace creation
{
    class BaseExtraList
    {
        BSExtraData *m_pBegin;
        BSExtraData **m_pEnd;
        uint8_t *m_pFlags;
    };
}
