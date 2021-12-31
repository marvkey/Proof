#include "SphereCollider.h"

namespace ProofPhysicsEngine {
	void SphereCollider::Transform(const Proof::Vector<> translation){
		Center += translation;
	}
	IntersectData SphereCollider::IntersectSphereCollider(const SphereCollider& other){
		float radiusDistance = Radius + other.Radius; // how fare away are they from touching
		float CenterDistance = (other.Center - Center).GetLength();
		float distance = CenterDistance - radiusDistance;
		return IntersectData( CenterDistance < radiusDistance, distance );
		//CenterDistance < radiusDistance // if it is equal the spheres are touched  
	}
}

