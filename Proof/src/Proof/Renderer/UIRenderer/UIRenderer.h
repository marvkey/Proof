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



	class ElevatedUIRenderer
	{
	public:
		static void BeginContext(Count<class Renderer2D> renderer2D,uint32_t width, uint32_t height,bool clearFrame = true);
		static void SubmitMenu(Count<class UIMenu> menu);
		static void EndContext();
	private:
		static void UICaclulateElement(Count<class UIMenu> menu, uint32_t screenWidth, uint32_t screenHeight, class UIElement element);
		static void UICaclulateMenuTransforms(Count<UIMenu> menu,uint32_t width, uint32_t height);
		static void StartFrame();
		static void EndFrame(); 
	};
}
