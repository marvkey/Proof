#pragma once
#include "Proof/Core/Core.h"
#include "Math.h"

namespace Proof
{
	struct Vector4
	{
		float X=0,Y=0,Z=0,W=0;
		Vector4(){};
		Vector4(float x, float y,float z,float w)
			:
			X(x),Y(y),Z(z),W(w)
		{
		}
		Vector4(const Vector4& other):
		X(other.X),Y(other.Y),Z(other.Z),W(other.W)
		{
		}
		
		float GetLength()const{
			return Math::SquareRoot<float>(X*X+Y*Y+Z*Z+W*W);
		}
		void Normalize(){
			float length = GetLength();
			X = X/length;
			Y = Y/length;
			Z = Z/length;
			W= W/length;
		}
		
		float Dot(const Vector4 other)const{
			return this->X * other.X + this->Y * other.Y + this->Z * other.Z* this->W* other.W;
		}
		float GetAngle(const Vector4& other) {
			float Angle = Dot(other) / GetLength() * other.GetLength();
			return Math::InverseCos<float>(Angle);
		}
		
		Vector4 operator+(const Vector4& other){
			Vector4 temp;
			temp.X = other.X+X;
			temp.Y = other.Y+Y;
			temp.Z = other.Z+Z;
			temp.W = other.W+W;
			return temp;
		}

		Vector4 operator+=(const Vector4& other){
			X += other.X;
			Y += other.Y;
			Z += other.Z;
			W += other.W;
			return *this;
		}

		Vector4 operator-(const Vector4& other) {
			Vector4 temp;
			temp.X = X-other.X;
			temp.Y = Y-other.Y;
			temp.Z = Z-other.Z;
			temp.W = W-other.W;
			return temp;
		}

		Vector4 operator-=(const Vector4& other) {
			X -= other.X;
			Y -= other.Y;
			Z -= other.Z;
			W -= other.W;
			return *this;
		}

		Vector4 operator*(float other){
			Vector4 temp;
			temp.X = X*other;
			temp.Y = Y*other;
			temp.Z = Z*other;
			temp.W = W*other;
			return temp;
		}

		Vector4 operator*=(float other) {
			X *= other;
			Y *= other;
			Z *= other;
			W *= other;
			return *this;
		}

		Vector4 operator/(float other) {
			Vector4 temp;
			temp.X = X / other;
			temp.Y = Y / other;
			temp.Z = Z / other;
			temp.W = W /other;
			return temp;
		}

		Vector4 operator/=(float other) {
			X /= other;
			Y /= other;
			Z /= other;
			W /= other;
			return *this;
		}
	};
}