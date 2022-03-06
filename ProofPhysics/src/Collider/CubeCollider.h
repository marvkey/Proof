#pragma once
#include "Collider.h"
#include "../IntersectData.h"
#include "../../../Proof/src/Proof/Resources/Math/Vector.h"
#include "../../../Proof/src/Proof/Resources/Math/Math.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
namespace ProofPhysicsEngine {
	class SphereCollider;
	class CubeCollider : public Collider {
	public:
		CubeCollider(const CubeCollider& other) = default;
		CubeCollider():
			Collider(ColliderType::Cube)
		{
			
		}
		CubeCollider(const Proof::Vector<float>& location, const Proof::Vector<float>& rotation, const Proof::Vector<float>& scale):
			Collider(ColliderType::Cube),
			Center(location), Rotation(rotation), Scale(scale)
		{

		}
		static float transformToAxis(const CubeCollider& box,
			const Proof::Vector<float>& axis) {
			return box.Center.X/2* axis.GetDistance(box.GetOrientation()[0]) +
				box.Center.Y / 2 * axis.GetDistance(box.GetOrientation()[1]) +
				box.Center.Z / 2 * axis.GetDistance(box.GetOrientation()[2]);
		}
		bool overlapOnAxis(
			const CubeCollider& one,
			const CubeCollider& two,
			const Proof::Vector<float>& axis
		) {
			// Project the half-size of one onto axis.
			float oneProject = transformToAxis(one, axis);
			float twoProject = transformToAxis(two, axis);
			// Find the vector between the two centers.
			Proof::Vector<float> toCenter = two.GetOrientation4()[3] - one.GetOrientation4()[3];
			// Project this onto the axis.
			float distance = axis.GetDistance(toCenter);
			// Check for overlap.
			return (distance < oneProject + twoProject);
		}
		Proof::Vector<float> Center = {0,0,0}; // location
		Proof::Vector<float> Rotation = { 0,0,0 };
		Proof::Vector<float> Scale = { 0,0,0 }; // TODO(MARV) maybe intilize as {1,1,1} in empty param constructor

		Proof::Vector<float> ClosestPoint(const Proof::Vector<float>& other)const;
		glm::mat3 GetOrientation()const {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, { Center.X,Center.Y,Center.Z });
			model = glm::rotate(model, Rotation.X, { 1,0,0 });// WE are NOT APPLYING Radians here 
			model = glm::rotate(model, Rotation.Y, { 0,1,0 });// WE are NOT APPLYING Radians here 
			model = glm::rotate(model, Rotation.Z, { 0,0,1 });// WE are NOT APPLYING Radians here 
			model = glm::scale(model, { Scale.X,Scale.Y,Scale.Z });

			return model;
		}
		glm::mat3 GetOrientation4()const {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, { Center.X,Center.Y,Center.Z });
			model = glm::rotate(model, Rotation.X, { 1,0,0 });// WE are NOT APPLYING Radians here 
			model = glm::rotate(model, Rotation.Y, { 0,1,0 });// WE are NOT APPLYING Radians here 
			model = glm::rotate(model, Rotation.Z, { 0,0,1 });// WE are NOT APPLYING Radians here 
			model = glm::scale(model, { Scale.X,Scale.Y,Scale.Z });

			return model;
		}
		virtual float GetVolume()const {
			return Scale.GetLength();
		}
		static unsigned boxAndPoint(
			const CubeCollider& box,
			const Proof::Vector<float>& point,
			CollisionData& data
		) {
			// Transform the point into box coordinates.
			Proof::Vector<float> relPt = point * box.GetOrientation();
			Proof::Vector<float> normal;

			// penetration is least deep.
			float min_depth = box.Center.X/2  -abs(relPt.X);
			if (min_depth < 0) return 0;
			normal = box.GetOrientation()[0]  * float((relPt.X < 0) ? -1 : 1);
			float depth = box.Center.Y/2- abs(relPt.Y);
			if (depth < 0) return 0;
			else if (depth < min_depth) {
				min_depth = depth;
				normal = box.GetOrientation()[1] * float((relPt.Y < 0) ? -1 : 1);
			}
			depth = box.Center.Z/2 - abs(relPt.Z);
			if (depth < 0) return 0;
			else if (depth < min_depth) {
				min_depth = depth;
				normal = box.GetOrientation()[2] * float((relPt.Z < 0) ? -1 : 1);
			}
			// Compile the contact.
			ContactData* contact = data.Contacts;
			contact->ContactNormal = normal;
			contact->ContactPoint = point;
			contact->penetration = min_depth;
			// Write the appropriate data.
			contact->body[0] = box.GetBody();
			// Note that we don’t know what rigid body the point
			// belongs to, so we just use NULL. Where this is called
			// this value can be left, or filled in.
			contact->body[1] = NULL;
			contact->Restitution = data.Restitution;
			contact->Friction = data.Friction;
			return 1;
		}
	
		IntersectData IntersectCubeCollider(const CubeCollider& other)const;
		IntersectData IntersectSphereCollider(const SphereCollider& other)const;

		static bool OverlappOnAxis(const CubeCollider& other1, const CubeCollider& other2, const Proof::Vector<float>& axis);
		Interval static GetInterval(const CubeCollider& other2, const Proof::Vector<float>& axis);
	};
}

