#pragma once

namespace creation
{
    struct NiColor
    {
        float r;
        float g;
        float b;
    };

    static_assert(sizeof(NiColor) == 0xC);
}
