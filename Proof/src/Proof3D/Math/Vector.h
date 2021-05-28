#pragma once
#include <glm/glm.hpp>
struct Proof_API Vector {
    Vector(float X,float Y,float Z);
    Vector() {};
    void SetX(float _X) { this->X = _X; };
    void SetY(float _Y) { this->Y = _Y; };
    void SetZ(float _Z) { this->Z = _Z; };

    void SetVector(Vector& _Vector) {
        this->X = _Vector.X;
        this->Y = _Vector.Y;
        this->Z = _Vector.Z;
    }
    void SetVector(float _X, float _Y, float _Z) {
        this->X = _X;
        this->Y = _Y;
        this->Z = _Z;
    }

    float X,Y,Z;
    operator const glm::vec3()const { return glm::vec3(X,Y,Z); }
    Vector operator=(glm::vec3 const & obj) {
        this->X = obj.x;
        this->Y = obj.y;
        this->Z = obj.z;
    }
};

