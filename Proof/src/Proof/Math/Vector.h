#pragma once
#include <glm/glm.hpp>
#include <sstream>
#include <string>
#include<iostream>>
namespace Proof
{
    struct Math;
    template<class T>
    struct VectorTemplate {
        T X, Y, Z;
        VectorTemplate() :
            X(T()), Y(T()), Z(T()) {
        };

        VectorTemplate(T val) {
            this->X = val, this->Y = val, this->Z = val;

        }
        VectorTemplate(const VectorTemplate& Other) {
            X = Other.X;
            Y = Other.Y;
            Z = Other.Z;
        }

        VectorTemplate(const glm::vec3& Other) {
            X = Other.x;
            Y = Other.y;
            Z = Other.z;
        }

        VectorTemplate(T x, T y, T z) {
            this->X = x, this->Y = y, this->Z = z;
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
        // transform all numbers to positve then finds the biggest 
        // finding the bsolute largest
        T GetMaxAbsolute() {
            VectorTemplate<T> vec = GetPositive();
            if (vec.X > Y) {
                if (vec.X > vec.Z)
                    return vec.X;
                return vec.Z;
            }
            if (vec.Y > vec.Z)
                return vec.Y;
            return vec.Z;
        }

       
        VectorTemplate<T> GetPositive()const {
            return VectorTemplate<T>{Math::GetPositive(X), Math::GetPositive(Y), Math::GetPositive(Z)};
        }
        std::string ToString()const {
            std::stringstream VecStream;
            VecStream << "X = " << X << " Y = " << Y << " Z = " << Z;
            return VecStream.str();
        }

        VectorTemplate Normalize()const {
            T Length = GetLength();
            VectorTemplate newVec;
            newVec.X = X / Length;
            newVec.Y = Y / Length;
            newVec.Z = Z / Length;
            return newVec;
        }


        VectorTemplate Cross(const VectorTemplate& Vec)const {
            VectorTemplate NewVec;
            NewVec.X = { this->Y * Vec.Z - this->Z * Vec.Y };
            NewVec.Y = { this->Z * Vec.X - this->X * Vec.Z };
            NewVec.Z = { this->X * Vec.Y - this->Y * Vec.X };
            return NewVec;
        }


        T Dot(const VectorTemplate& Vec)const {
            return this->X * Vec.X + this->Y * Vec.Y + this->Z * Vec.Z;
        }

        T GetLength()const {
            return Math::SquareRoot<T>(X * X + Y * Y + Z * Z);
        }
        // basically the dot product
        T GetLengthSquared()const {
            return X * X + Y * Y + Z * Z;
        }

        T GetAngle(const VectorTemplate& Vec)const {
            T Angle = Dot(*this, Vec) / (GetLength(*this) * GetLength(Vec));
            return Math::DInverseCos<T>(Angle);
        }


        operator const glm::vec3()const { return glm::vec3(X, Y, Z); }
        VectorTemplate operator=(glm::vec3 const& obj) {
            X = obj.x;
            Y = obj.y;
            Z = obj.z;
            return *this;
        }

        VectorTemplate operator&(glm::vec3 const& obj) {
            this->X = obj.x;
            this->Y = obj.y;
            this->Z = obj.z;
            return *this;
        }

        VectorTemplate operator +=(glm::vec3& Other) {
            this->X += Other.x;
            this->Y += Other.y;
            this->Z += Other.z;
            return *this;
        }
        VectorTemplate operator -=(const glm::vec3& Other) {
            this->X -= Other.x;
            this->Y -= Other.y;
            this->Z -= Other.z;
            return *this;
        }

        void AddScaledVectorTemplate(VectorTemplate& other, float number) {
            this += other * number;
        }

        T GetDistance(const VectorTemplate& other)const {
            return sqrt(pow(X - other.X, 2) + pow(Y - other.Y, 2) + pow(Z - other.Z, 2));
        }
        T* GetValue_Ptr() {
            return &X;
        }
    };
    template<typename T>
    VectorTemplate<T> operator*(const VectorTemplate<T>& other, const T& temp) {
        VectorTemplate<T>tempVec(other.X * temp, other.Y * temp, other.Z * temp);
        return tempVec;
    }

    template<typename T>
    VectorTemplate<T> operator*(const VectorTemplate<T>& other, const VectorTemplate<T>& other1) {
        VectorTemplate<T>tempVec(other.X * other1.X, other.Y * other1.Y, other.Z * other1.Z);
        return tempVec;
    }
    template<typename T>
    inline VectorTemplate<T> operator*(const T& temp, const VectorTemplate<T>& other) {
        VectorTemplate<T> tempVec(other.X * temp, other.Y * temp, other.Z * temp);
        return tempVec;
    }
    template<typename T>
    inline VectorTemplate<T> operator *=(const T& temp, VectorTemplate<T>& other) {
        other.X *= temp;
        other.Y *= temp;
        other.Z *= temp;
        return *other;
    }

    template<typename T>
    inline VectorTemplate<T> operator -=(VectorTemplate<T>& other, const T& temp) {
        other.X -= temp;
        other.Y -= temp;
        other.Z -= temp;
        return other;
    }
    template<typename T>
    VectorTemplate<T> operator +(const VectorTemplate<T>& Other1, const VectorTemplate<T>& Other2) {
        VectorTemplate<T> Temp(Other1.X + Other2.X, Other1.Y + Other2.Y, Other1.Z + Other2.Z);
        return Temp;
    }
    
    template<typename T>
    inline VectorTemplate<T> operator +=(VectorTemplate<T>& other, const T& temp) {
        other.X += temp;
        other.Y += temp;
        other.Z += temp;
        return other;
    }

    template<typename T>
    VectorTemplate<T> operator -(const VectorTemplate<T>& Other1, const VectorTemplate<T>& Other2) {
        VectorTemplate<T> Temp(Other1.X - Other2.X, Other1.Y - Other2.Y, Other1.Z - Other2.Z);
        return Temp;
    }

    template<typename T>
    VectorTemplate<T> operator +=(VectorTemplate<T>& other1, const VectorTemplate<T>& other2) {
        other1.X += other2.X;
        other1.Y += other2.Y;
        other1.Z += other2.Z;
        return other1;
    }

    template<class T>
    struct VectorTemplate4 {
        T X, Y, Z, W;
        VectorTemplate4() :
            X(T()), Y(T()), Z(T()),W(T()) {
        };
        VectorTemplate4(T x, T y, T z, T w)
            :
            X(x), Y(y), Z(z), W(w) {
        }
        VectorTemplate4(const VectorTemplate4& other) :
            X(other.X), Y(other.Y), Z(other.Z), W(other.W) {
        }

        T GetLength()const {
            return Math::SquareRoot<T>(X * X + Y * Y + Z * Z + W * W);
        }
        VectorTemplate4 Normalize() {
            T Length = GetLength();
            VectorTemplate4 newVec;
            newVec.X = X / Length;
            newVec.Y = Y / Length;
            newVec.Z = Z / Length;
            newVec.W = W / Length;
            return newVec;
        }

        T Dot(const VectorTemplate4 other)const {
            return this->X * other.X + this->Y * other.Y + this->Z * other.Z * this->W * other.W;
        }
        T GetAngle(const VectorTemplate4& other) {
            T Angle = Dot(other) / GetLength() * other.GetLength();
            return Math::InverseCos<T>(Angle);
        }

    };
    template<typename T>
    VectorTemplate4<T> operator*(const VectorTemplate4<T>& other, const T& temp) {
        VectorTemplate4<T>tempVec(other.X * temp, other.Y * temp, other.Z * temp,other.W* temp);
        return tempVec;
    }

    template<typename T>
    VectorTemplate4<T> operator*(const VectorTemplate4<T>& other, const VectorTemplate4<T>& other1) {
        VectorTemplate4<T>tempVec(other.X * other1.X, other.Y * other1.Y, other.Z * other1.Z, other.W * other1.W);
        return tempVec;
    }
    template<typename T>
    inline VectorTemplate4<T> operator*(const T& temp, const VectorTemplate4<T>& other) {
        VectorTemplate4<T>tempVec(other.X * temp, other.Y * temp, other.Z * temp, other.W * temp);
        return tempVec;
    }

    template<typename T>
    inline VectorTemplate4<T> operator *=(const T& temp, VectorTemplate4<T>& other) {
        other.X *= temp;
        other.Y *= temp;
        other.Z *= temp;
        other.W *= temp;
        return *other;
    }
    template<typename T>
    inline VectorTemplate4<T> operator -=(VectorTemplate4<T>& other, const T& temp) {
        other.X -= temp;
        other.Y -= temp;
        other.Z -= temp;
        other.W -= temp;
        return other;
    }
    template<typename T>
    VectorTemplate4<T> operator +(const VectorTemplate4<T>& Other1, const VectorTemplate4<T>& Other2) {
        VectorTemplate4<T> Temp(Other1.X + Other2.X, Other1.Y + Other2.Y, Other1.Z + Other2.Z, Other1.W + Other2.W);
        return Temp;
    }
    template<typename T>
    VectorTemplate4<T> operator -(const VectorTemplate4<T>& Other1, const VectorTemplate4<T>& Other2) {
        VectorTemplate4<T> Temp(Other1.X - Other2.X, Other1.Y - Other2.Y, Other1.Z - Other2.Z, Other1.W - Other2.W);
        return Temp;
    }

    template<class T>
    struct VectorTemplate2 {
        T X, Y;
        VectorTemplate2() :
            X(T()), Y(T()){
        };
        VectorTemplate2(T x, T y)
            :
            X(x), Y(y) {
        }
        VectorTemplate2(const VectorTemplate2& other) :
            X(other.X), Y(other.Y) {
        }

        T GetLength()const {
            return Math::SquareRoot<T>(X * X + Y * Y);
        }
        T Normalize() {
            T length = GetLength();
            X = X / length;
            Y = Y / length;
        }

        T Dot(const VectorTemplate2& other)const {
            return this->X * other.X + this->Y * other.Y;
        }
        T GetAngle(const VectorTemplate2& other) {
            T Angle = Dot(other) / GetLength() * other.GetLength();
            return Math::InverseCos<T>(Angle);
        }
    };
    template<typename T>
    bool operator!=(const VectorTemplate2<T>& other, const VectorTemplate2<T>& other1) {
        if ((other.X != other1.X) || (other.Y != other1.Y))
            return true;
        return false;
    }
    template<typename T>
    VectorTemplate2<T> operator*(const VectorTemplate2<T>& other, const T& temp) {
        VectorTemplate2<T>tempVec(other.X * temp, other.Y * temp);
        return tempVec;
    }

    template<typename T>
    VectorTemplate2<T> operator*(const VectorTemplate2<T>& other, const VectorTemplate2<T>& other1) {
        VectorTemplate2<T>tempVec(other.X * other1.X, other.Y * other1.Y);
        return tempVec;
    }
    template<typename T>
    inline VectorTemplate2<T> operator*(const T& temp, const VectorTemplate2<T>& other) {
        VectorTemplate2<T>tempVec(other.X * temp, other.Y * temp);
        return tempVec;
    }

    template<typename T>
    inline VectorTemplate2<T> operator *=(const T& temp, VectorTemplate2<T>& other) {
        other.X *= temp;
        other.Y *= temp;
        return *other;
    }
    template<typename T>
    inline VectorTemplate2<T> operator -=(VectorTemplate2<T>& other, const T& temp) {
        other.X -= temp;
        other.Y -= temp;
        return other;
    }
    template<typename T>
    VectorTemplate2<T> operator +(const VectorTemplate2<T>& Other1, const VectorTemplate2<T>& Other2) {
        VectorTemplate4<T> Temp(Other1.X + Other2.X, Other1.Y + Other2.Y);
        return Temp;
    }
    template<typename T>
    VectorTemplate2<T> operator -(const VectorTemplate2<T>& Other1, const VectorTemplate2<T>& Other2) {
        VectorTemplate2<T> Temp(Other1.X - Other2.X, Other1.Y - Other2.Y);
        return Temp;
    }
 
    using Vector2 = VectorTemplate2<float>;
    using Vector = VectorTemplate<float>;
    using Vector4 = VectorTemplate4<float>;
    
    using ScreenSize = VectorTemplate2<uint32_t>;
}