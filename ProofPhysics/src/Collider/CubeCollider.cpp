#include "CubeCollider.h"
#include "SphereCollider.h"
namespace ProofPhysicsEngine {
	IntersectData CubeCollider::IntersectCubeCollider(const CubeCollider& other)const{
		float distance = (other.Center - Center).GetLength(); // the distance calculated may not be entirely right
		glm::mat3 thisOrientation = GetOrientation();
		glm::mat3 otherOrientation = other.GetOrientation();
		Proof::Vector<float> test[15] = {
			Proof::Vector<>(thisOrientation[0]),
			Proof::Vector<>(thisOrientation[1]),
			Proof::Vector<>(thisOrientation[2]),
			Proof::Vector<>(otherOrientation[0]),
			Proof::Vector<>(otherOrientation[1]),
			Proof::Vector<>(otherOrientation[2]),
		};
	
		for (int i = 0; i < 3; i++) { // fill the remaining of test
			test[6 + i * 3 + 0] = Proof::Vector<float>::Cross(test[i], test[0]);
			test[6 + i * 3 + 1] = Proof::Vector<float>::Cross(test[i], test[1]);
			test[6 + i * 3 + 2] = Proof::Vector<float>::Cross(test[i], test[2]);
		}
		for (int i = 0; i < 15; ++i) {
			if (!OverlappOnAxis(*this, other, test[i])) {
				return IntersectData(false, distance); // Seperating axis found
			}
			
		}
		return IntersectData(true, distance);
	}
	IntersectData CubeCollider::IntersectSphereCollider(const SphereCollider& other)const{
		Proof::Vector<float> closestPoint = ClosestPoint(other.Center);
		float distance = (other.Center - closestPoint).GetLength();
		return IntersectData(distance < other.Radius, distance);
	}
	bool CubeCollider::OverlappOnAxis(const CubeCollider& other1, const CubeCollider& other2, const Proof::Vector<float>& axis)
	{
		Interval a = GetInterval(other1, axis);
		Interval b = GetInterval(other2, axis);
		return ((b.min <= a.max) && (a.min <= b.max));
	}
	Interval CubeCollider::GetInterval(const CubeCollider& other, const Proof::Vector<float>& axis){
		static Proof::Vector<float> vertex[8]; // we are not reallocation everytime we call
		Proof::Vector<float> center = other.Center;
		Proof::Vector<float> extent = other.Scale;
		glm::mat3 boxOrientation = other.GetOrientation();
		static Proof::Vector<float> A[]{
			Proof::Vector<float>(boxOrientation[0]),
			Proof::Vector<float>(boxOrientation[1]),
			Proof::Vector<float>(boxOrientation[2]),
		};
		vertex[0] = center + A[0] * extent.X + A[1] * extent.Y + A[2] * extent.Z;
		vertex[1] = center - A[0] * extent.X + A[1] * extent.Y + A[2] * extent.Z;
		vertex[2] = center + A[0] * extent.X - A[1] * extent.Y + A[2] * extent.Z;
		vertex[3] = center + A[0] * extent.X + A[1] * extent.Y - A[2] * extent.Z;
		vertex[4] = center - A[0] * extent.X - A[1] * extent.Y - A[2] * extent.Z;
		vertex[5] = center + A[0] * extent.X - A[1] * extent.Y - A[2] * extent.Z;
		vertex[6] = center - A[0] * extent.X + A[1] * extent.Y - A[2] * extent.Z;
		vertex[7] = center - A[0] * extent.X - A[1] * extent.Y + A[2] * extent.Z;

		Interval result;
		result.min = result.max = Proof::Vector<float>::Dot(axis, vertex[0]);
		for (int i = 1; i < 8; ++i) {
			float projection = Proof::Vector<float>::Dot(axis, vertex[i]);
			result.min = (projection < result.min) ?
				projection : result.min;
			result.max = (projection > result.max) ?
				projection : result.max;
		}
		return result;
	}
	Proof::Vector<float> CubeCollider::ClosestPoint(const Proof::Vector<float>& other)const{
		Proof::Vector<float> result = Center;
		glm::mat3 boxOrientation = GetOrientation();
		Proof::Vector<float> direction = other - Center;
		for (int i = 0; i < 3; i++) {
			Proof::Vector<float> axis = boxOrientation[i];
			float distance = Proof::Vector<float>::Dot(direction, axis);

			// clamping the component Scale if needed // (UNDERSTAND)
			if (i == 0) {
				if (distance > Scale.X)
					distance = Scale.X;

				if (distance < -Scale.X)
					distance = -Scale.X;
			}
			else if(i == 1) {
				if (distance > Scale.Y)
					distance = Scale.Y;

				if (distance < -Scale.Y)
					distance = -Scale.Y;
			}
			else if (i == 2) {
				if(distance > Scale.Z)
					distance = Scale.Z;

				if (distance < -Scale.Z)
					distance = -Scale.Z;
			}
			result = result + (axis * distance);
		}
		return result;
	}
}