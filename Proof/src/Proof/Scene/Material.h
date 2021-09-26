#pragma once

#include "Proof/Core/Core.h"
#include "Proof/Resources/Math/Math.h"

namespace Proof{
	struct Proof_API Material {
		//glm::vec3 m_Ambient = {1.0,1.0f,1.0f};
		//glm::vec3 m_Diuffuse ={1.0,1.0f,1.0f};
		//glm::vec3 m_Specular ={1.0,1.0f,1.0f};
		float m_Metallness =0.0f;
		float m_Roughness = 0.0f;
		float m_AO=1.0f;
		glm::vec3 m_Colour={1,1,1};
		//std::vector<Texture2D> m_Textures;
		Count<Texture2D>NormalTexture;
		Count<Texture2D>AlbedoTexture;
		Count<Texture2D>MetallicTexture;
		Count<Texture2D>RoughnessTexture;
	};
}