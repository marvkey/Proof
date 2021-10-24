#pragma once

#include "Proof/Core/Core.h"
#include "Proof/Resources/Math/Math.h"

namespace Proof{
	struct Proof_API Material {
		float m_Metallness =0.0f;
		float m_Roughness = 0.0f;
		float m_AO=1.0f;
		glm::vec3 m_Colour={1,1,1};
		Count<class Texture2D>NormalTexture;
		Count<class Texture2D>AlbedoTexture;
		Count<class Texture2D>MetallicTexture;
		Count<class Texture2D>RoughnessTexture;
	};
}