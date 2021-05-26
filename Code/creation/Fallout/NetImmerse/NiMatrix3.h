#pragma once

#include <glm/mat3x3.hpp>

namespace creation
{
    // the game only stores a collection of NiPoint4's but
    // it is faster to operate on glm mat object directly
    class NiMatrix3 : public glm::mat4x3
    {
    public:
        inline NiMatrix3() : glm::mat4x3() {}
        inline NiMatrix3(const glm::mat4x3& kMatrix) : glm::mat4x3(kMatrix) {}
    };

    static_assert(sizeof(NiMatrix3) == 0x30);
}
