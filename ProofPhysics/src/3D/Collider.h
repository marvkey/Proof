#pragma once
#include "../../../Proof/vendor/glm/glm/glm.hpp"
namespace ProofPhysics{

	class Collider{
	public:
		void CheckCollision(glm::mat4& transform,class SphereCollider*collider,glm::mat4& colliderTransform)=0;
		void CheckCollision(glm::mat4& transform,class PlaneCollider*collider,glm::mat4& colliderTransform)=0;
	};
}