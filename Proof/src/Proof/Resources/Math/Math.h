#pragma once
#include "Proof/Core/Core.h"
#include <cmath>
#include "Vector.h"
#include "Rotate.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Proof
{
	struct Proof_API Math {
		Math(Math&) = delete;
		/**
		* @param Number
		* @return the square root of the param Number
		*/
		static float SquareRoot(float Number) {
			return sqrtf(Number);
		}
		/**
		* @param Num1
		* @param Num3
		* @return the biggest value betwen Num1 and Num2
		*/
		template<typename T>
		inline static T Min(T Num1,T Num2) {
			return Num1 < Num2 ? Num1 : Num2;
		}
		/**
		* @param Num1
		* @param Num3
		* @return the Smallest value betwen Num1 and Num2
		*/
		template<typename T>
		inline static T Max(T Num1,T Num2) {
			return Num1 > Num2 ? Num1 : Num2;
		}
		/**
		* @param Value
		* @return changes value to a cos
		*/
		inline static float Cos(float Value) {
			return cos(Value);
		}
		/**
		* @param Value
		* @return convert value to a sin
		*/
		inline static float Sin(float Value) {
			return sin(Value);
		}
		/**
		* @param Value
		* @return convert value to a Tan
		*/
		inline static float Tan(float Value) {
			return tan(Value);
		}

		/**
		* @param Value
		* @return convert value to a inverse sin and returns in degrees
		*/
		inline static float InverseSine(float Value) {
			return Degrees(asin(Value));
		}
		/**
		* @param Value
		* @return convert value to a inverse cos and returns in degrees
		*/
		inline static float InverseCos(float Value) {
			return Degrees(acos(Value));
		}
		/**
		* @param Value
		* @return convert value to a inverse Tan and returns in degrees
		*/
		inline static float InverseTan(float Value) {
			return Degrees(atan(Value));
		}
		/**
		* @param Radian: value to be changed
		* @return The paremeter Radius inot converted Degrees
		*/
		inline static float Degrees(float Radian) {
			return Radian * (180 / GetPIE());
		}
		/**
		* @param Degree: value to be changed
		* @return The paremeter Degree inot converted Radian
		*/
		inline static float Radian(float Degree) {
			return Degree * (GetPIE() / 180);
		}
		// @return the numerical value PIE
		inline static float GetPIE() {
			return 3.141592654;
		}

		/*
		* generates a random number
		* @param Min: minimum number generated
		* @ max: maximum number gnerated
		*/
		inline static uint32_t RandUINT(uint32_t Min,uint32_t Max) {
			if (Min == 0)
				return rand() % (Max + 1);
			return Min + (rand() % Max);
		}
	};
}