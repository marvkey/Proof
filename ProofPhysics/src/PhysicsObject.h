#pragma once
#include <glm/vec3.hpp>
#include "../../Proof/src/Proof/Resources/Math/Vector.h"
#include "Collider/SphereCollider.h"
namespace ProofPhysicsEngine {
	class PhysicsObject // rename PhysicsBody
	{
	public:
		PhysicsObject(Collider& collider){
			m_Collider = &collider;
		}
		void Update(float delta);
		inline Collider* GetCollider() {
			return m_Collider;
		}
	private:
		friend class PhysicsWorld;
		Collider* m_Collider;
	};
}