#pragma once
#include <glm/glm.hpp>
#include <sstream>
#include <string>
#include<iostream>>
namespace Proof
{
    struct Math;
    template<class T=float>
    struct Vector {
        T X = 0,Y = 0,Z = 0;
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
        
        Vector(T x, T y,T z) {
            this->X = x,this->Y = y,this->Z = z;
        }
        
        T GetMax() {
            if (X > Y) {
                if (X > Z)
                    return X;
                return Z;
            }
            if (Y > Z)
                return Y;
            return Z;
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
            T Length = GetLength();
            X = X / Length;
            Y = Y / Length;
            Z = Z / Length;
            return *this;
        }

        static Vector Normalize(Vector& Vec) {
            T Length = Vec.GetLength(); 
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
        static T Dot(const Vector& Vec1,const Vector& Vec2) {
            return Vec1.X * Vec2.X + Vec1.Y * Vec2.Y + Vec1.Z * Vec2.Z;
        }

        T Dot(const Vector& Vec) {
            return this->X * Vec.X + this->Y * Vec.Y + this->Z * Vec.Z;
        }

        T GetLength()const {
            return Math::SquareRoot<T>(X * X + Y * Y + Z * Z);
        }
        // basically the dot product
        T GetLengthSquared()const { 
            return X * X + Y * Y + Z * Z;
        }
        // basically the dot product
        static T GetLengthSquared(const Vector& Vec) {
            return Vec.X * Vec.X + Vec.Y * Vec.Y + Vec.Z * Vec.Z;
        }
        static T GetLength(const Vector& Vec) {
            return  Math::SquareRoot<T>(Vec.X * Vec.X + Vec.Y * Vec.Y + Vec.Z * Vec.Z);
        }

        T GetAngle(const Vector& Vec)const {
            T Angle = Dot(*this,Vec) / (GetLength(*this) * GetLength(Vec));
            return Math::DInverseCos<T>(Angle);
        }

        static T GetAngle(const Vector& Vec1,const Vector& Vec2) {
            T Angle = Dot(Vec1,Vec2) / (GetLength(Vec1) * GetLength(Vec2));
            return Math::DInverseCos<T>(Angle);
        }

        operator const glm::vec3()const { return glm::vec3(X,Y,Z); }
        Vector operator=(glm::vec3 const& obj) {
            X = obj.x;
            Y = obj.y;
            Z = obj.z;
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
        Vector<T> operator +=(const Vector<T>& Other) {
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
        Vector operator *=(const float& temp) {
            this->X *= temp;
            this->Y *= temp;
            this->Z *= temp;
            return *this;
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
    template<typename T>
    Vector<T> operator*(const Vector<T>& other,float temp) {
        Vector<T>tempVec(other.X * temp,other.Y * temp,other.Z * temp);
        return tempVec;
    }
    
    template<typename T>
    inline Vector<T> operator*(const float& temp, const Vector<T>& other) {
        Vector<T> tempVec(other.X * temp, other.Y * temp, other.Z * temp);
        return tempVec;
    }
    template<typename T>
    void AddScaledVector(Vector<T>& other, float number) {
        this += other * number;
    }
    template<typename T>
    inline Vector<T> operator *=(const float& temp, Vector<T>& other) {
        other.X *= temp;
        other.Y *= temp;
        other.Z *= temp;
        return *other;
    }
    template<typename T>
    Vector<T> operator +(const Vector<T>& Other1, const Vector<T>& Other2){
        Vector<T> Temp(Other1.X + Other2.X, Other1.Y + Other2.Y, Other1.Z + Other2.Z);
        return Temp;
    }
    template<typename T>
    Vector<T> operator -(const Vector<T>& Other1, const Vector<T>& Other2) {
        Vector<T> Temp(Other1.X - Other2.X, Other1.Y - Other2.Y, Other1.Z - Other2.Z);
        return Temp;
    }
}