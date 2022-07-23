#pragma once
#include "Entity.h"
#include <vector>
#include "Proof/Resources/Math/Math.h"
namespace Proof{
	enum class ColliderTypes {
		CubeCollider = 0,
		SphereCollider,
		CapsuleCollider,
		MeshCollider
	};
	struct ContactPointInfo {
		//normal of touching surfaces at contact point
		Vector<float> Normal;
		//positon of contact world space
		Vector<float> Location;
		// distance contact point
		float Distance; 
		// impule contact point
		Vector<float> Impulse;
	};
	struct CollisionInfo{
		Entity otherEntity;
		std::vector<ContactPointInfo> ContactPoints;
		ColliderTypes OtherColliderType;
		uint32_t NumContacts;	
	};

	struct TriggerInfo {
		ColliderTypes OtherColliderType;
		Entity otherEntity;
	};

}
