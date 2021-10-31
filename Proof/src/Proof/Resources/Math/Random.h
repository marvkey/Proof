#pragma once
#include <random>
#include "Proof/Core/Core.h"
static  std::random_device rd;
static std::mt19937 s_RandomEngine;
//static std::uniform_int_distribution<int>  rand;
namespace Proof
{
	struct Random{
	public:
		/* call this funciton to set the seed at the beggiing of your proggram */
		static void RandomInit(){
			s_RandomEngine.seed(rd());
			//rand = std::uniform_int_distribution<int>(2,2);
		};
		inline static bool RandBool(){
			std::uniform_int_distribution<int> dist(0,1);
			return dist(s_RandomEngine);
		}

		inline static float RandFloat(float min,float max){
			std::uniform_real_distribution<float> dist(min,max);
			return dist(s_RandomEngine);
		}
		template<typename T>
		inline static T RandInt(T minNumber,T maxNumber){
			PF_ASSERT(minNumber < maxNumber,"Minimum cannnto be bigger than max");
			std::uniform_int_distribution<T> dist(minNumber,maxNumber);
			return dist(s_RandomEngine);
		}

		template<typename T>
		inline static T RandUint(T minNumber, T maxNumber){
			PF_ASSERT(false,"function not created");
			return (((T)(unsigned int)rand() << 32) + (T)(unsigned int)rand()) % (maxNumber - minNumber) + minNumber;
		}

		inline static double RandDouble(double min, double max){
			std::uniform_real_distribution<double> dist(min,max);
			return dist(s_RandomEngine);
		}
	};

}