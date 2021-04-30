#pragma once
#include <glm/glm.hpp>
namespace Proof {
    struct Proof_API Scale3{
        Scale3(float X, float Y, float Z) :
            X(X), Y(Y), Z(Z)
        {
        }
        Scale3() {
        }
        float X=1.0f, Y=1.0f, Z=1.0f;
        operator const  glm::vec3()const { return glm::vec3(X, Y, Z);}
    };
}


