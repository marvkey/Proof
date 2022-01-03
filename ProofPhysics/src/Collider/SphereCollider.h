#pragma once
#include<glm/vec3.hpp>
#include "../IntersectData.h"
#include "Collider.h"
#include "../../../Proof/src/Proof/Resources/Math/Vector.h"

namespace ProofPhysicsEngine {
	class CubeCollider;
	class SphereCollider : public Collider
	{
	public:
		SphereCollider():
			Collider(ColliderType::Sphere) 
		{
			
		}
		SphereCollider(const Proof::Vector<>& center, float radius) :
			Collider(ColliderType::Sphere),	
			Center(center), 
			Radius(radius)
		{
		}
		IntersectData IntersectSphereCollider(const SphereCollider& other);
		IntersectData IntersectCubeCollider(const CubeCollider& other);
		Proof::Vector<> Center = {0,0,0};
		float Radius = 1;
	};
}

