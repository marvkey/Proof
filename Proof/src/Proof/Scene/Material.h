#pragma once

#include "Proof/Core/Core.h"
#include "Proof/Resources/Math/Math.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
namespace Proof{
	struct Proof_API Material {
		glm::vec3 m_Ambient = {1.0,1.0f,1.0f};
		glm::vec3 m_Diuffuse ={1.0,1.0f,1.0f};
		glm::vec3 m_Specular ={1.0,1.0f,1.0f};
		float m_Metallness =1.0f;
	};
}