#pragma once
#include<glm/vec3.hpp>
#include "IntersectData.h"
namespace ProofPhysicsEngine {
	class AABB{
	public:
		AABB(const glm::vec3& minExtents, const glm::vec3& maxExtents) :
			m_MinExtents(minExtents),m_MaxExtents(maxExtents)
		{

		}
		IntersectData IntersectAABB(const AABB& other) const;
		glm::vec3 m_MinExtents;
		glm::vec3 m_MaxExtents;
	};
}
