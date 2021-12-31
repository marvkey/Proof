#pragma once
#include "../IntersectData.h"
#include "../../../Proof/src/Proof/Resources/Math/Vector.h"

namespace ProofPhysicsEngine {
	class Collider {
	public:
		bool IsTrigger = false;
		Collider() = default;
		enum class ColliderType {
			None =0,
			Sphere,
			Cube,
			AABB
		};
		ColliderType GetType() {
			return m_Type;
		}
		IntersectData  Intersect(const Collider& other)const;
		virtual void Transform(const Proof::Vector<> translation) {};
	protected:
		Collider(ColliderType type) :m_Type(type) {

		}
		ColliderType m_Type = ColliderType::None;
	};
}