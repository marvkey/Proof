#pragma once
#include <cmath>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "Random.h"
namespace Proof
{
	struct  Math {
		Math(const Math&) = delete;
		/**
		* @param Number
		* @return the square root of the Number
		*/
		template<typename T>
		inline static T SquareRoot(T Number) {
			return sqrtf(Number);
		}

		/*
		* swaps the 2 variables
		* @param variable1: varible assign to 2
		* @param variable2: variable assign to 1
		*/
		template<class T>
		inline static void Swap(T& Variable1,T& Variable2){
			T temp= Variable1;
			Variable1 = Variable2;
			Variable2 =temp;
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
		* @return changes value to a cos in Radians
		*/
		template<typename T>
		inline static T RCos(T Value) {
			return cos(Value);
		}
		/**
		* @param Value
		* @return convert value to a sin in Radians
		*/
		template<typename T>
		inline static T RSin(T Value) {
			return sin(Value);
		}
		/**
		* @param Value
		* @return convert value to a Tan in Radians
		*/
		template<typename T>
		inline static T RTan(T Value) {
			return tan(Value);
		}

		/**
		* @param Value
		* @return changes value to a cos in Degrees
		*/
		template<typename T>
		inline static T DCos(T Value) {
			return Degrees<T>(cos(Value));
		}
		/**
		* @param Value
		* @return convert value to a sin in Degrees
		*/
		template<typename T>
		inline static T DSin(T Value) {
			return Degrees<T>(sin(Value));
		}
		/**
		* @param Value
		* @return convert value to a Tan in Degrees
		*/
		template<typename T>
		inline static T DTan(T Value) {
			return Degrees<T>(tan(Value));
		}
		/**
		* @param Value
		* @return convert value to a inverse sin and returns in degrees
		*/
		template<typename T>
		inline static T DInverseSine(T Value) {
			return Degrees<T>(asin(Value));
		}
		/**
		* @param Value
		* @return convert value to a inverse cos and returns in degrees
		*/
		template<typename T>
		inline static T DInverseCos(T Value) {
			return Degrees<T>(acos(Value));
		}
		/**
		* @param Value
		* @return convert value to a inverse Tan and returns in degrees
		*/
		template<typename T>
		inline static T DInverseTan(T Value) {
			return Degrees<T>(atan(Value));
		}

		/**
		* @param Value
		* @return convert value to a inverse sin and returns in Radians
		*/
		template<typename T>
		inline static T RInverseSine(T Value) {
			return asin(Value);
		}
		/**
		* @param Value
		* @return convert value to a inverse cos and returns in Radians
		*/
		template<typename T>
		inline static T RInverseCos(T Value) {
			return acos(Value);
		}
		/**
		* @param Value
		* @return convert value to a inverse Tan and returns in Radians
		*/
		template<typename T>
		inline static T RInverseTan(T Value) {
			return atan(Value);
		}
		/**
		* @param Radian: value to be changed
		* @return The paremeter Radius inot converted Degrees
		*/
		template<typename T>
		inline static T Degrees(T Radian) {
			return Radian * (180 / PIE());
		}
		/**
		* @param Degree: value to be changed
		* @return The paremeter Degree inot converted Radian
		*/
		template<typename T>
		inline static T Radian(T Degree) {
			return Degree * (PIE() / 180);
		}
		// @return the numerical value PIE
		inline static float PIE() {
			return 3.14159265359;
		}
		// @return the max value of a specific type
		template<typename T>
		inline static T GetMaxType() {
			return std::numeric_limits<T>::max();
		}
		// @return the mi value of a specific type
		template<typename T>
		inline static T GetMinType() {
			return std::numeric_limits<T>::min();
		}
	
	};
}