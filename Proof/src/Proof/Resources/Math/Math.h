#pragma once
#include "Proof/Core/Core.h"
#include <cmath>
#include "Vector.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Proof
{
	struct Proof_API Math {
		Math(const Math&) = delete;
		/**
		* @param Number
		* @return the square root of the param Number
		*/
		template<typename T>
		static T SquareRoot(T Number) {
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
		template<typename T>
		inline static T Cos(T Value) {
			return cos(Value);
		}
		/**
		* @param Value
		* @return convert value to a sin
		*/
		template<typename T>
		inline static T Sin(T Value) {
			return sin(Value);
		}
		/**
		* @param Value
		* @return convert value to a Tan
		*/
		template<typename T>
		inline static T Tan(T Value) {
			return tan(Value);
		}

		/**
		* @param Value
		* @return convert value to a inverse sin and returns in degrees
		*/
		template<typename T>
		inline static T InverseSine(T Value) {
			return Degrees(asin(Value));
		}
		/**
		* @param Value
		* @return convert value to a inverse cos and returns in degrees
		*/
		template<typename T>
		inline static T InverseCos(T Value) {
			return Degrees<T>(acos(Value));
		}
		/**
		* @param Value
		* @return convert value to a inverse Tan and returns in degrees
		*/
		template<typename T>
		inline static T InverseTan(T Value) {
			return Degrees<T>(atan(Value));
		}
		/**
		* @param Radian: value to be changed
		* @return The paremeter Radius inot converted Degrees
		*/
		template<typename T>
		inline static T Degrees(T Radian) {
			return Radian * (180 / GetPIE());
		}
		/**
		* @param Degree: value to be changed
		* @return The paremeter Degree inot converted Radian
		*/
		template<typename T>
		inline static T Radian(T Degree) {
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