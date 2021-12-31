#include "Plane.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
namespace ProofPhysicsEngine {
	//IntersectData Plane::IntersectSphere(const BoundingSphere& other) const{
	//	// dot product gets us how far a point is from another poitn 
	//	// adding distance helps us know distnace from spehr to the plain
	//	float distanceFromSphereCenter = fabs(Proof::Vector<>::Dot(Normal, other.Center) + Distance);
	//	float distanceFromSphere = distanceFromSphereCenter	- other.Radius;
	//	return IntersectData(distanceFromSphere < 0, distanceFromSphere);
	//}
	Plane Plane::Normalized() const{
		float	magnitude = Normal.length();

		return Plane(Normal/magnitude,Distance/magnitude);
	}
}