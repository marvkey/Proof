#pragma once
#include <glm/glm.hpp>
namespace Proof {
    struct Proof_API Rotate3 {
        Rotate3(float X, float Y, float Z);
        Rotate3() {}
        operator const  glm::vec3()const  { return glm::vec3(X, Y, Z);}
        bool operator ==(const Rotate3& Rotate) {
            if (Rotate.X == this->X && Rotate.Y == this->Y && Rotate.Z == this->Z)
                return true;
            return false;
        }
        float X = 0.0f, Y =0.0f, Z = 0.0f;
    };
}