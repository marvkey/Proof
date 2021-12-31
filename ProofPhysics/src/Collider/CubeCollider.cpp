#include "CubeCollider.h"

namespace ProofPhysicsEngine {
	IntersectData CubeCollider::IntersectCubeCollider(const CubeCollider& other){
		float distance = (other.Center - Center).GetLength();

		return IntersectData(false, 0);
	}
}