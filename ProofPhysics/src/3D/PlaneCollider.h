#pragma once
#include "../../../Proof/vendor/glm/glm/glm.hpp"
#include "Collider.h"
namespace ProofPhysics
{

	class PlaneCollider: public Collider {
	public:
		virtual void CheckCollision(glm::mat4& transform,class SphereCollider* collider,glm::mat4& colliderTransform);
		virtual void CheckCollision(glm::mat4& transform,class PlaneCollider* collider,glm::mat4& colliderTransform);
	};
}