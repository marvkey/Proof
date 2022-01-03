#pragma once
#include "../IntersectData.h"
#include "../../../Proof/src/Proof/Resources/Math/Vector.h"
#include <functional>
namespace ProofPhysicsEngine {
	enum class ColliderType {
		None = 0,
		Sphere,
		Cube,
		AABB
	};
	class Collider {
	public:
		bool IsTrigger = false;
		Collider() = default;
		
		ColliderType GetType() {
			return m_Type;
		}
		IntersectData  Intersect(const Collider& other)const;

		void SetCollisionCallback(std::function<void(Collider&)>& callback) { 
			m_ColliderCallBack = &callback;
		}

		
	protected:
		std::function<void(Collider&)>* m_ColliderCallBack =nullptr;

		Collider(ColliderType type) :m_Type(type) {

		}
		ColliderType m_Type = ColliderType::None;
	};
}