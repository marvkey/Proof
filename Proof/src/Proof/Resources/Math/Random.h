#pragma once
#include <random>
#include "Proof/Core/Core.h"
static	std::random_device s_RandomDevice;
static	std::mt19937 s_RandomEngine(s_RandomDevice());
static	std::mt19937_64 s_RandomEngine64(s_RandomDevice());
namespace Proof
{
	struct Random {
	public:
		
		inline static bool RandBool() {
			std::uniform_int_distribution<int> dist(0,1);
			return dist(s_RandomEngine);
		}

		inline static float RandFloat(float min,float max) {
			std::uniform_real_distribution<float> dist(min,max);
			return dist(s_RandomEngine);
		}
		template<typename T>
		inline static T RandInt(T minNumber,T maxNumber) {
			std::uniform_int_distribution<T> dist(minNumber,maxNumber);
			return dist(s_RandomEngine);
		}

		template<typename T>
		inline static T RandUint(T minNumber,T maxNumber) {
			std::uniform_int_distribution<T> dist(minNumber,maxNumber);
			return dist(s_RandomEngine);
		}
		template<>
		inline static uint64_t RandUint(uint64_t minNumber,uint64_t maxNumber) {
			std::uniform_int_distribution<uint64_t> dist(minNumber,maxNumber);
			return dist(s_RandomEngine64);

		}
		inline static double RandDouble(double min,double max) {
			std::uniform_real_distribution<double> dist(min,max);
			return dist(s_RandomEngine);
		}
	};
}