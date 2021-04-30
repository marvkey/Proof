#pragma once
#include <glm/glm.hpp>
namespace Proof {
    struct Proof_API Vector3{
        Vector3(float X, float Y, float Z);
        float X, Y, Z;
        operator const  glm::vec3()const { return glm::vec3(X, Y, Z); }
    };
}

