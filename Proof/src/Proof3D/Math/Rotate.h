#pragma once
#include <glm/glm.hpp>

struct Proof_API Rotate {
    Rotate(float X,float Y,float Z);
    Rotate() {}
    void  SetX(float _X) { X = _X; };
    void  SetY(float _Y) { Y = _Y; };
    void  SetZ(float _Z) { Z = _Z; };
    operator const  glm::vec3()const { return glm::vec3(X,Y,Z); }
    bool operator ==(const Rotate& Rotate) {
        if (Rotate.X == this->X && Rotate.Y == this->Y && Rotate.Z == this->Z)
            return true;
        return false;
    }

    float X = 0.0f,Y = 0.0f,Z = 0.0f;
};
