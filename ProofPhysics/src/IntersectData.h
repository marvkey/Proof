#pragma once
#include "../../../Proof/src/Proof/Resources/Math/Vector.h"
#include "../../../Proof/src/Proof/Resources/Math/Math.h"
#include "Body/RigidBody.h"
namespace ProofPhysicsEngine {
	struct Interval {
		float min; float max;
	};
	struct IntersectData
	{
	public:
		IntersectData(bool intersects, float distance) :
			m_DoesIntersect(intersects), m_Distance(distance)
		{
		}
		inline bool DoesIntersect()const {
			return m_DoesIntersect;
		}
		inline float GetDistance()const {
			return m_Distance;
		}
	private:
		const bool m_DoesIntersect;
		const float m_Distance;
	};

	struct ContactData {
		/**
		* Position of contact
		*/
		Proof::Vector<float> ContactPoint;
		/**
		* Holds the direction of the contact in world coordinates.
		*/
		Proof::Vector<float>  ContactNormal;
		/**
		* Holds the depth of penetration at the contact point. If both
		* bodies are specified then the contact point should be midway
		* between the inter-penetrating points.
		*/
		float penetration;
		RigidBody* body[2];

		float Restitution;
		float Friction;
	};

	struct CollisionData {

		/** Holds the contact array to write into. */
		ContactData* Contacts;
		/** Holds the maximum number of contacts the array can take. */
		unsigned ContactsLeft;

		float Restitution;
		float Friction;
	};
}
