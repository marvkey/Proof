#pragma once
#include <glm/vec3.hpp>

namespace Proof
{
	struct AABB;
	struct Ray;
	class BasicCollision
	{
		static bool PointInAABB(const AABB& aabb, const glm::vec3& point);
		static bool AABBInAABB(const AABB& aabb0, const AABB& aabb1);
		static bool RayInAABB(const Ray& Ray,const AABB& aabb0, float& tmin, float& tmax);
		static bool RayInAABB(const Ray& Ray, const AABB& aabb, float& t);
	};
}