#pragma once
#include<glm/vec3.hpp>
#include "../IntersectData.h"
#include "Collider.h"
#include "../../../Proof/src/Proof/Resources/Math/Vector.h"

namespace ProofPhysicsEngine {
	class SphereCollider : public Collider
	{
	public:
		SphereCollider() = default;
		SphereCollider(const Proof::Vector<>& center, float radius) :
			Collider(ColliderType::Sphere),	
			Center(center), 
			Radius(radius)
		{
		}
		virtual void Transform(const Proof::Vector<> translation)override;
		IntersectData IntersectSphereCollider(const SphereCollider& other);
		Proof::Vector<> Center;
		float Radius;
		
	};
}

