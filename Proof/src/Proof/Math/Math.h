#pragma once
#include <cmath>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include  <algorithm>
#include <limits>
#include "Vector.h"
namespace Proof
{
	struct Math 
	{
		Math(const Math&) = delete;

		inline static glm::vec3 GetFowardVector()
		{
			return { 0.0f, 0.0f, -1.0f };
		}

		inline static glm::vec3 GetUpVector()
		{
			return { 0.0f, 1.0f, 0.0f };
		}

		inline static glm::vec3 GetRightVector()
		{
			return { 1.0f, 0.0f, 0.0f };
		}
		/**
		* @param Number
		* @return the square root of the Number
		*/
		template<typename T>
		inline static T SquareRoot(T Number) {
			return sqrtf(Number);
		}

		inline static void ChangeBool(bool& val) {
			if (val == true)
				val = false;
			else
				val = true;
		}
		inline static glm::vec4 Lerp(const glm::vec4& start, const glm::vec4& end, float t)
		{
			t = std::clamp(t, 0.0f, 1.0f);  // Ensure t is clamped between 0 and 1
			glm::vec4 result;
			result.x = start.x + (end.x - start.x) * t;
			result.y = start.y + (end.y - start.y) * t;
			result.z = start.z + (end.z - start.z) * t;
			result.w = start.w + (end.w - start.w) * t;
			return result;
		}

		inline static glm::vec3 Lerp(const glm::vec3& a, const glm::vec3& b, float t)
		{
			t = std::clamp(t, 0.0f, 1.0f);  // Ensure t is clamped between 0 and 1
			glm::vec4 result;
			result.x = a.x + (b.x - a.x) * t;
			result.y = a.y + (b.y - a.y) * t;
			result.z = a.z + (b.z - a.z) * t;
			return result;
		}

		inline static Vector Lerp(const Vector& a, const Vector& b, float t)
		{
			t = std::clamp(t, 0.0f, 1.0f);  // Ensure t is clamped between 0 and 1
			Vector result;
			result.X = a.X + (b.X - a.X) * t;
			result.Y = a.Y + (b.Y - a.Y) * t;
			result.Z = a.Z + (b.Z - a.Z) * t;
			return result;
		}
		static float Lerp(float start, float end, float t) {
			t = std::clamp(t, 0.0f, 1.0f);  // Ensure t is clamped between 0 and 1
			return start + (end - start) * t;
		}
		// returns teh opposite of a boleaon values
		inline static bool InverseBool(bool val) {
			if (val == true)return false;
			return true;
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
			//180/PIE() = 57.295779513082320876798154814105
			return Radian * static_cast<T>(57.295779513082320876798154814105);
		}
		/**
		* @param Degree: value to be changed
		* @return The paremeter Degree inot converted Radian
		*/
		template<typename T>
		inline static T Radian(T Degree) {
			//PIE()/180 = 0.01745329251994329576923690768489
			return Degree * static_cast<T>(0.01745329251994329576923690768489);
		}
		// @return the numerical value PIE
		constexpr inline static float PIE() {
			return 3.14159265358979323846264338327950288;
		}
		// @return the max value of a specific type
		template<typename T>
		inline constexpr static T GetMaxType() {
			return std::numeric_limits<T>::max();
		}
		// @return the min value of a specific type
		template<typename T>
		inline constexpr static T GetMinType() {
			return std::numeric_limits<T>::min();
		}
		template<>
		inline static float GetMinType() {
			return std::numeric_limits<float>::lowest();
		}

		template<>
		inline static double GetMinType() {
			return std::numeric_limits<double>::lowest();
		}

		template<>
		inline static long double GetMinType() {
			return std::numeric_limits<long double>::lowest();
		}
		/**
		* @param num: value to be changed
		* @return The number passed into positve
		*/
		template<typename T>
		inline static T GetPositive(T num) {
			if (num < 0)
				num = -num;
			return num;
		}

		// make sure quaternion values are in radians

		// values are returned in degrees
		static Vector ConvertQuartToVector(const glm::quat& q) {

			//Math::Pie()/2 = 1.57079632679489661923132169163975144
		//https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
			Vector angles;

			// roll (x-axis rotation)
			double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
			double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
			angles.X = std::atan2(sinr_cosp, cosr_cosp);

			// pitch (y-axis rotation)
			double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
			double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
			angles.Y = 2 * std::atan2(sinp, cosp) - 1.57079632679489661923132169163975144;

			// yaw (z-axis rotation)
			double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
			double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
			angles.Z = std::atan2(siny_cosp, cosy_cosp);

			return { Math::Degrees(angles.X),Math::Degrees(angles.Y),Math::Degrees(angles.Z) };
		};

		static inline float Repeat(float t, float length)
		{
			return glm::modf(t, length);
		}
		static inline float DeltaAngle(float currentRadians, float targetRadians)
		{
			float num = Repeat(currentRadians - targetRadians, glm::radians(360.0f));
			if (num > glm::radians((180.0f)))
			{
				num -= glm::radians(360.0f);
			}
			return num;
		}

		//TODO: Replace with a C++20 concept?
		template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
		inline static T DivideAndRoundUp(T dividend, T divisor)
		{
			return (dividend + divisor - 1) / divisor;
		}

		// Only integers are allowed
		template<typename T, typename DivisorT, std::enable_if_t<std::is_same_v<T, glm::uvec2>&& std::is_integral_v<DivisorT>, bool> = true>
		inline static T DivideAndRoundUp(T dividend, DivisorT divisor)
		{
			return { DivideAndRoundUp(dividend.x, divisor), DivideAndRoundUp(dividend.y, divisor) };
		}
	};
}