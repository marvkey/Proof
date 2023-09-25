#pragma once
#include <glm/glm.hpp>

namespace Proof
{

    struct Ray {
        glm::vec3 Origin;
        glm::vec3 Direction;

        Ray(const glm::vec3& origin, const glm::vec3& direction)
            : Origin(origin), Direction(glm::normalize(direction)) {}


        static Ray Zero()
        {
            return { {0.0f, 0.0f, 0.0f},{0.0f, 0.0f, 0.0f} };
        }
    };

}   