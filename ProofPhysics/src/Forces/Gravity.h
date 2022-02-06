#pragma once
#include "../../../Proof/src/Proof/Resources/Math/Vector.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "../Body/RigidBody.h"
namespace ProofPhysicsEngine
{
	struct GravityForce {
		Proof::Vector<float> Gravity = { 0,-9.8,0 };
		void UpdateForce(RigidBody& body) {
			if (body.IsMassInfinite() || body.Gravity ==false)return; // has infinite mass no need ot calculate
			body.AddForce(Gravity * body.GetMass());
		}
	};
}