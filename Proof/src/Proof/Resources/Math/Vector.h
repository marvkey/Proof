#pragma once
#include <glm/glm.hpp>
namespace Proof
{
    struct Proof_API Vector {
        float X = 0,Y = 0,Z = 0;

        Vector(float X,float Y,float Z);
        Vector(int X,int Y,int Z)     {
            this->X = (float)X;
            this->Y = Y;
            this->Z = Z;

        }

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
        std::string ToString();
        static std::string ToString(const Vector& Vec);

        Vector Normalize();
        static Vector Normalize(Vector& Vec);

        float Dot(const Vector& Vec);
        static float Dot(const Vector& Vec1,const Vector& Vec2);

        Vector Cross(const Vector& Vec);
        static Vector Cross(const Vector& Vec1,const Vector& Vec2);

        float GetLength();
        static float GetLength(const Vector& Vec);

        float GetAngle(const Vector& Vec);
        static float GetAngle(const Vector& Vec1,const Vector& Vec2);

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

        Vector operator -=(const glm::vec3& Other) {
            this->X -= Other.x;
            this->Y -= Other.y;
            this->Z -= Other.z;
            return *this;
        }
        Vector operator -(const glm::vec3& Other) {
            Vector Temp(this->X - Other.x,this->Y - Other.y,this->Z - Other.z);
            return Temp;
        }
        Vector operator +(const glm::vec3& Other) {
            Vector Temp(this->X + Other.x,this->Y + Other.y,this->Z + Other.z);
            return Temp;
        }

        Vector operator -(const Vector& Other) {
            Vector Temp(this->X - Other.X,this->Y - Other.Y,this->Z - Other.Z);
            return Temp;
        }

        Vector operator *(const glm::vec3& Other) {
            Vector Temp(this->X * Other.x,this->Y * Other.y,this->Z * Other.z);
            return Temp;
        }


        Vector operator *(const Vector& Other) {
            Vector Temp(this->X * Other.X,this->Y * Other.Y,this->Z * Other.Z);
            return Temp;
        }
    };

}