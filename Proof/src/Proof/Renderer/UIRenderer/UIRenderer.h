#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
namespace Proof
{

	class UIRenderer 
	{
	public:
		static void DrawUI(Count<class UIMenu> menu, Count<class Renderer2D> renderer,uint32_t width, uint32_t height);
		static void DrawElement(Count<class UIMenu> menu, Count<class Renderer2D> renderer,uint32_t width, uint32_t height,class UIElement element);
	};
}
