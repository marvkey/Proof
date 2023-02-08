#pragma once
#include<glm/glm.hpp>
#include "Vector.h"
namespace Proof{

	static glm::vec3 ProofToglmVec(const Vector& vec) {
		return glm::vec3{ vec.X,vec.Y,vec.Z };
	}
	static Vector GlmVecToProof(const glm::vec3& vec) {
		return Vector{ vec.x,vec.y,vec.z };
	}
}
