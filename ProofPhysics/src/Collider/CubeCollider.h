#pragma once
#include "Collider.h"
#include "../IntersectData.h"
#include "../../../Proof/src/Proof/Resources/Math/Vector.h"
namespace ProofPhysicsEngine {

	class CubeCollider : public Collider {
	public:
		
		CubeCollider():
			Collider(ColliderType::Cube)
		{

		}
		IntersectData IntersectCubeCollider(const CubeCollider& other);
		Proof::Vector<float> Center;
		Proof::Vector<float> Size;
		Proof::Vector<float> Rotation;
	};
}

