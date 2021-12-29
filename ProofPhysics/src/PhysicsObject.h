#pragma once
#include <glm/vec3.hpp>
#include "../../Proof/src/Proof/Resources/Math/Vector.h"
#include "BoundingSphere.h"
namespace ProofPhysicsEngine {
	class PhysicsObject
	{
	public:
		PhysicsObject(Proof::Vector<>* position, Proof::Vector<>* velocity, float *radius)
		{
			Position = position;
			Veclocity =velocity;
			m_Radius = radius;
		}
		void Update(float delta);
		inline BoundingSphere GetBoudingSphere()const {
			return BoundingSphere(glm::vec3(*Position), *m_Radius);
		}
	private:
		Proof::Vector<>* Position =nullptr;
		Proof::Vector<>* Veclocity =nullptr;
		float *m_Radius = nullptr;
		friend class PhysicsEngine;
	};
}