#pragma once
#include<glm/vec3.hpp>
#include "IntersectData.h"
namespace Proof {
	class BoundingSphere
	{
		BoundingSphere(const glm::vec3 center, float radius) :
			Center(center), Radius(radius)
		{
		}
		IntersectData IntersectBoundingSphere(const BoundingSphere& other);
		glm::vec3 Center;
		float Radius;
	};
}

