#pragma once
namespace Proof
{
	enum class DataType {
		Int,
		Uint32_t, 
		Uint64_t,
		Float,
		Double,
		Char,

		//signed int vec2
		Ivec2,
		//signed int vec3
		Ivec3,
		//signed int vec4
		Ivec4,


		//vector 2 with float
		Vec2,
		// vector 3 with float
		Vec3,
		//vector 4 with float
		Vec4,

		//vector 2 with double
		DVec2,
		// vector 3 with double
		DVec3,
		//vector 4 with double
		DVec4,

		//usnigned vec4 32 bit
		UVec4,
	};
}
