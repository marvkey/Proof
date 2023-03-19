#pragma once
#include "Entity.h"
#include <vector>
#include "Proof/Math/Math.h"
namespace Proof{
	enum class ColliderTypes {
		CubeCollider = 0,
		SphereCollider,
		CapsuleCollider,
		MeshCollider
	};
	struct ContactPointInfo {
		//normal of touching surfaces at contact point
		Vector Normal;
		//positon of contact world space
		Vector Location;
		// distance contact point
		float Distance; 
		// impule contact point
		Vector Impulse;
	};
	struct ContactInfo{
		Entity otherEntity;
		std::vector<ContactPointInfo> ContactPoints;
		ColliderTypes OtherColliderType;
		uint32_t NumContacts;	
	};

	// like thi because of c# scripitng 
	// so use Id for the entity
	struct CollisonInfo 
	{
		UUID Entity;
	};

	struct TriggerInfo {
		ColliderTypes OtherColliderType;
		Entity otherEntity;
	};

}
