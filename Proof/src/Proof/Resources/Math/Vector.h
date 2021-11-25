#pragma once
#include <glm/glm.hpp>
#include <sstream>
#include "Math.h"
#include <string>
#include<iostream>>
namespace Proof
{
    struct Proof_API Vector {
        float X = 0,Y = 0,Z = 0;

        Vector() {};
        
        Vector(const Vector& Other) {
            X = Other.X;
            Y = Other.Y;
            Z = Other.Z;
        }
        
        Vector(const glm::vec3& Other) {
            X = Other.x;
            Y = Other.y;
            Z = Other.z;
        }
        
        Vector(float X,float Y,float Z) {
            this->X = X,this->Y = Y,this->Z = Z;
        }
        
        std::string ToString()const {
            std::stringstream VecStream;
            VecStream << "X = " << X << " Y = " << Y << " Z = " << Z;
            return VecStream.str();
        }
        std::string ToString(const Vector& Vec) {
            std::stringstream VecStream;
            VecStream << "X = " << Vec.X << " Y = " << Vec.Y << " Z = " << Vec.Z;
            return VecStream.str();
        }
        
        Vector Normalize() {
            float Length = GetLength();
            X = X / Length;
            Y = Y / Length;
            Z = Z / Length;
            return *this;
        }

        static Vector Normalize(Vector& Vec) {
            float Length = Vec.GetLength();
            Vec.X = Vec.X / Length;
            Vec.Y = Vec.Y / Length;
            Vec.Z = Vec.Z / Length;
            return Vec;
        }

        Vector Cross(const Vector& Vec) {
            Vector NewVec;
            NewVec.X = {this->Y * Vec.Z - this->Z * Vec.Y};
            NewVec.Y = {this->Z * Vec.X - this->X * Vec.Z};
            NewVec.Z = {this->X * Vec.Y - this->Y * Vec.X};
            return NewVec;
        }

        static Vector Cross(const Vector& Vec1,const Vector& Vec2) {
            Vector NewVec;
            NewVec.X = {Vec1.Y * Vec2.Z - Vec1.Z * Vec2.Y};
            NewVec.Y = {Vec1.Z * Vec2.X - Vec1.X * Vec2.Z};
            NewVec.Z = {Vec1.X * Vec2.Y - Vec1.Y * Vec2.X};
            return NewVec;
        }
        static float Dot(const Vector& Vec1,const Vector& Vec2) {
            return Vec1.X * Vec2.X + Vec1.Y * Vec2.Y + Vec1.Z * Vec2.Z;
        }

        float Dot(const Vector& Vec) {
            return this->X * Vec.X + this->Y * Vec.Y + this->Z * Vec.Z;
        }

        float GetLength()const {
            return Math::SquareRoot<float>(X * X + Y * Y + Z * Z);
        }

        static float GetLength(const Vector& Vec) {
            return  Math::SquareRoot<float>(Vec.X * Vec.X + Vec.Y * Vec.Y + Vec.Z * Vec.Z);
        }

        float GetAngle(const Vector& Vec)const {
            float Angle = Dot(*this,Vec) / (GetLength(*this) * GetLength(Vec));
            return Math::DInverseCos<float>(Angle);
        }

        static float GetAngle(const Vector& Vec1,const Vector& Vec2) {
            float Angle = Dot(Vec1,Vec2) / (GetLength(Vec1) * GetLength(Vec2));
            return Math::DInverseCos<float>(Angle);
        }

        operator const glm::vec3()const { return glm::vec3(X,Y,Z); }
        Vector operator=(glm::vec3 const& obj) {
            this->X = obj.x;
            this->Y = obj.y;
            this->Z = obj.z;
            return *this;
        }

        Vector operator&(glm::vec3 const& obj) {
            this->X = obj.x;
            this->Y = obj.y;
            this->Z = obj.z;
            return *this;
        }

        Vector operator =(const Vector& Other) {
            this->X = Other.X;
            this->Y = Other.Y;
            this->Z = Other.Z;
            return *this;
        }

        Vector operator +=(glm::vec3& Other) {
            this->X += Other.x;
            this->Y += Other.y;
            this->Z += Other.z;
            return *this;
        }
        Vector operator +=(Vector& Other) {
            this->X += Other.X;
            this->Y += Other.Y;
            this->Z += Other.Z;
            return *this;
        }
        Vector operator -=(const glm::vec3& Other) {
            this->X -= Other.x;
            this->Y -= Other.y;
            this->Z -= Other.z;
            return *this;
        }
        Vector operator -(const glm::vec3& Other)const {
            Vector Temp(this->X - Other.x,this->Y - Other.y,this->Z - Other.z);
            return Temp;
        }
        Vector operator +(const glm::vec3& Other)const {
            Vector Temp(this->X + Other.x,this->Y + Other.y,this->Z + Other.z);
            return Temp;
        }

        Vector operator -(const Vector& Other)const {
            Vector Temp(this->X - Other.X,this->Y - Other.Y,this->Z - Other.Z);
            return Temp;
        }

        Vector operator +(const Vector& Other) {
            Vector Temp(this->X + Other.X,this->Y + Other.Y,this->Z + Other.Z);
            return Temp;
        }

        Vector operator-()const{
            Vector temp(-this->X,-this->Y,-this->Z);
            return temp;
        }

        Vector operator +=(float other) {
            this->X+=other;
            this->Y+=other;
            this->Z+=other;
            return *this;
        }
    };
    /*
    Vector operator*(const Vector& other,float temp) {
        Vector tempVec(other.X * temp,other.Y * temp,other.Z * temp);
        return tempVec;
    }
    */
    inline Vector operator*(float temp,const Vector& other) {
        Vector tempVec(other.X * temp,other.Y * temp,other.Z * temp);
        return tempVec;
    }
}