#pragma once

#include <glm/glm.hpp>

namespace creation
{
    // We implement our niPoint on top of glm
    // since it provides everything we need
    class NiPoint3 final : public glm::vec3
    {
    public:
        inline NiPoint3() : glm::vec3() {}
        inline NiPoint3(float x, float y, float z) : glm::vec3(x, y, z) {};
        inline NiPoint3(const NiPoint3& kSrc) : glm::vec3(kSrc) {}

    };

    static_assert(sizeof(NiPoint3) == sizeof(float) * 3);
}
