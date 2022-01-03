#include "CubeCollider.h"
#include "SphereCollider.h"
namespace ProofPhysicsEngine {
	IntersectData CubeCollider::IntersectCubeCollider(const CubeCollider& other)const{
		float distance = (other.Center - Center).GetLength();

		return IntersectData(false, 0);
	}
	IntersectData CubeCollider::IntersectSphereCollider(const SphereCollider& other)const{
		Proof::Vector<float> Point = ClosestPoint(other.Center);
		float distance = (other.Center - Point).GetLength();
		return IntersectData(distance < other.Radius, distance);
	}
	Proof::Vector<float> CubeCollider::ClosestPoint(const Proof::Vector<float>& other)const{
		glm::vec3 result = other;
		glm::mat3 boxOrientation = GetOrientation();
		glm::vec3 direction = other - Center;
		for (int i = 0; i < 3; i++) {
			glm::vec3 axis = boxOrientation[i];
			float distance = glm::dot(direction, axis);

			// clamping the component axis if needed // (UNDERSTAND)
			if (distance > glm::length(axis))
				distance = glm::length(axis);

			if (distance < -glm::length(axis))
				distance = -glm::length(axis);
			result = result + (axis * distance);
		}
		return result;
	}
}