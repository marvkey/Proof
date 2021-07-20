#include "Proofprch.h"
#include "Vector.h"
#include "Math.h"
#include <sstream>
namespace Proof
{
    Vector::Vector(float X,float Y,float Z) {
        this->X = X,this->Y = Y,this->Z = Z;
    }
    std::string Vector::ToString() {
        std::stringstream VecStream;
        VecStream << "X = " << X << " Y = " << Y << " Z = " << Z;
        return VecStream.str();
    }
    std::string Vector::ToString(const Vector& Vec) {
        std::stringstream VecStream;
        VecStream << "X = " << Vec.X << " Y = " << Vec.Y << " Z = " << Vec.Z;
        return VecStream.str();
    }
    Vector Vector::Normalize() {
        float Length = GetLength();
        X = X / Length;
        Y = Y / Length;
        Z = Z / Length;
        return *this;
    }

    Vector Vector::Normalize(Vector& Vec) {
        float Length = Vec.GetLength();
        Vec.X = Vec.X / Length;
        Vec.Y = Vec.Y / Length;
        Vec.Z = Vec.Z / Length;
        return Vec;
    }

    Vector Vector::Cross(const Vector& Vec) {
        Vector NewVec;
        NewVec.X = {this->Y * Vec.Z - this->Z * Vec.Y};
        NewVec.Y = {this->Z * Vec.X - this->X * Vec.Z};
        NewVec.Z = {this->X * Vec.Y - this->Y * Vec.X};
        return NewVec;
    }

    Vector Vector::Cross(const Vector& Vec1,const Vector& Vec2) {
        Vector NewVec;
        NewVec.X = {Vec1.Y * Vec2.Z - Vec1.Z * Vec2.Y};
        NewVec.Y = {Vec1.Z * Vec2.X - Vec1.X * Vec2.Z};
        NewVec.Z = {Vec1.X * Vec2.Y - Vec1.Y * Vec2.X};
        return NewVec;
    }
    float Vector::Dot(const Vector& Vec1,const Vector& Vec2) {
        return Vec1.X * Vec2.X + Vec1.Y * Vec2.Y + Vec1.Z * Vec2.Z;
    }

    float Vector::Dot(const Vector& Vec) {
        return this->X * Vec.X + this->Y * Vec.Y + this->Z * Vec.Z;
    }

    float Vector::GetLength() {
        return Math::SquareRoot(X * X + Y * Y + Z * Z);
    }

    float Vector::GetLength(const Vector& Vec) {
        return  Math::SquareRoot(Vec.X * Vec.X + Vec.Y * Vec.Y + Vec.Z * Vec.Z);
    }

    float Vector::GetAngle(const Vector& Vec) {
        float Angle = Vector::Dot(*this,Vec);
        Angle = Angle / (GetLength(*this) * GetLength(Vec));
        return Math::InverseCos(Angle);
    }

    float Vector::GetAngle(const Vector& Vec1,const Vector& Vec2) {
        float Angle = Vector::Dot(Vec1,Vec2);
        Angle = Angle / (GetLength(Vec1) * GetLength(Vec2));
        return Math::InverseCos(Angle);
    }
}