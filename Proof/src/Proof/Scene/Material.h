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
		
		Material()
			:Colour(1.0f)
		{
		};
		Material(const glm::vec4& color){
			this->Colour = color;
		}

		glm::vec4 Colour;
		glm::vec4 CaluclateMaterial(){
			return Colour;
		}
	};
}