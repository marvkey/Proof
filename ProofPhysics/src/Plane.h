#pragma once
#include<glm/vec3.hpp>
#include "IntersectData.h"
namespace ProofPhysicsEngine {
	class Plane
	{
	public:
		Plane(const glm::vec3& normal, float distance) :
			Normal(normal),Distance(distance)
		{

		}
		//IntersectData IntersectSphere(const BoundingSphere& other)const;
		Plane Normalized()const;
		glm::vec3 Normal; /// the orienteation of the plain
		float Distance; // how far along the normal the plain is
	};
}

