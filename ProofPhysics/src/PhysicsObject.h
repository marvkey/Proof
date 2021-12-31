#pragma once
#include <glm/vec3.hpp>
#include "../../Proof/src/Proof/Resources/Math/Vector.h"
#include "Collider/SphereCollider.h"
namespace ProofPhysicsEngine {
	class PhysicsObject
	{
	public:
		PhysicsObject(Proof::Vector<>* position, Proof::Vector<>* velocity, float *radius)
		{
			Position = position;
			Veclocity =velocity;
			m_Radius = radius;
			m_SphereCollider = { glm::vec3(*Position),*radius };
			m_oldPosition = *position;
		}
		void Update(float delta);
		inline const Collider& GetBoudingSphere(){
			Proof::Vector translation = *Position - m_oldPosition;
			m_SphereCollider.Transform(translation);
			m_oldPosition = *Position;
			return m_SphereCollider;
		}
	private:
		Proof::Vector<>* Position =nullptr;
		Proof::Vector<>* Veclocity =nullptr;
		float *m_Radius = nullptr;
		 Proof::Vector<> m_oldPosition;
		friend class PhysicsWorld;
		mutable SphereCollider m_SphereCollider;
	};
}