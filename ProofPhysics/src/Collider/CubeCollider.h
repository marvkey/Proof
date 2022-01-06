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

	
		IntersectData IntersectCubeCollider(const CubeCollider& other)const;
		IntersectData IntersectSphereCollider(const SphereCollider& other)const;

		static bool OverlappOnAxis(const CubeCollider& other1, const CubeCollider& other2, const Proof::Vector<float>& axis);
		Interval static GetInterval(const CubeCollider& other2, const Proof::Vector<float>& axis);
	};
}

