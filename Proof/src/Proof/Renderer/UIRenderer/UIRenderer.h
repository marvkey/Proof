#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Texture.h"
namespace Proof{

	class UiRenderer {
	public:
		static Image DrawUI(Count<class UIPanel> panel, glm::vec3 cameraLocaion, glm::mat4 projectionMatrix, glm::mat4 viewProjection, uint32_t width, uint32_t height);

	};
}
