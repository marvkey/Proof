#pragma once
#include <glm/vec3.hpp>
namespace Proof
{
	struct AABB
	{
		glm::vec3 Min, Max;

		AABB()
			: Min(std::numeric_limits<float>::infinity()), Max(-std::numeric_limits<float>::infinity()) {}

		AABB(const glm::vec3& min, const glm::vec3& max)
			: Min(min), Max(max) {}

	};
}