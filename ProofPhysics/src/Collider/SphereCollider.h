#pragma once
#include<glm/vec3.hpp>
#include "../IntersectData.h"
#include "Collider.h"
#include "../../../Proof/src/Proof/Resources/Math/Vector.h"
#include "../../../Proof/src/Proof/Resources/Math/Math.h"

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

		static uint32_t SphereAndCube(const SphereCollider& one, const CubeCollider& two, CollisionData& data);
		static uint32_t SphereAndSphere(const SphereCollider& one, const SphereCollider& two, CollisionData& data);
		virtual float GetVolume()const {
			//VOLUME OF SPHERE =
			//					4/3(PIE)Radius^3
			return (4 / 3)* Proof::Math::PIE()* Radius* Radius* Radius;
		}
		// position
		Proof::Vector<> Center = {0,0,0};
		float Radius = 1;
	};
}

