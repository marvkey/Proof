#pragma once
#include "../../../Proof/src/Proof/Resources/Math/Vector.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include <vector>
namespace ProofPhysicsEngine
{
	struct Plane {
		Proof::Vector<> Position;
		Proof::Vector<> Direction;
	};
	template<class Object>
	using BSPObjectSet = std::vector<Object>;
	
	enum class BSPCHildType {
		Node,
		Object
	};
	struct BSPChild {
		BSPCHildType type;
		union  {
			BSPNode* node;
			BSPObjectSet<int>* object; // int is tempory
		};
	};
	struct BSPNode {
		Plane plane;
		BSPChild Front;
		BSPChild Back;
	};
};