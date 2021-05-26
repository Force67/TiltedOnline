#pragma once

namespace creation
{
    struct NiColor
    {
        float r;
        float g;
        float b;
    };

    static constexpr NiColor BLACK{ 0.f, 0.f, 0.f };
    static constexpr NiColor WHITE{ 1.f, 1.f, 1.f };

    static_assert(sizeof(NiColor) == 0xC);
}
