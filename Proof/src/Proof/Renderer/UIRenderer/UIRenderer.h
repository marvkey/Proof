#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
namespace Proof
{

	class UIRenderer 
	{
	public:
		static void DrawUI(Count<class UIPanel> panel, Count<class Renderer2D> renderer, const glm::mat4& projectionMatrix, const glm::mat4 viewProjection, uint32_t width, uint32_t height);
		static void DrawElement(Count<class UIPanel> panel, Count<class Renderer2D> renderer,const glm::mat4& projectionMatrix, const glm::mat4 viewProjection, uint32_t width, uint32_t height,class UIElement element);
	};
}
