#include "BoundingSphere.h"

namespace Proof {
	IntersectData BoundingSphere::IntersectBoundingSphere(const BoundingSphere& other){
		float radiusDistance = Radius + other.Radius; // how fare away are they from touching
		float CenterDistance = (other.Center - Center).length();
		float distance = CenterDistance < radiusDistance;
		return IntersectData{ CenterDistance < radiusDistance, distance };
		//CenterDistance < radiusDistance // if it is equal the spheres are touched  
	}
}

