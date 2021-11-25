#pragma once
#include <random>
static	std::random_device s_RandomDevice;
static	std::mt19937_64 s_RandomEngine64(s_RandomDevice());
namespace Proof{
	struct Random {
		inline static bool Bool() {
			std::uniform_int_distribution<int> dist(0,1);
			return dist(s_RandomEngine64);
		}
		template<typename T>
		inline static T Real(T min = std::numeric_limits<T>::min(), T max = std::numeric_limits<T>::max()) {
			std::uniform_real_distribution<T> dist(min, max);
			return dist(s_RandomEngine64);
		};
		template<typename T>
		inline static T Int(T minNumber= std::numeric_limits<T>::min(),T maxNumber= std::numeric_limits<T>::max()) {
			std::uniform_int_distribution<T> dist(minNumber, maxNumber);
			return dist(s_RandomEngine64);
		}
	};
}