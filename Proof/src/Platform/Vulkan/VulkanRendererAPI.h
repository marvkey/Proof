#pragma once
#include "Proof/Renderer/RendererAPI.h"
#include "VulkanGraphicsContext.h"
#include "Platform/Window/CurrentWindow.h"
namespace Proof {
	class VulkanRendererAPI: public RendererAPI {
		virtual void SetClearColor(const glm::vec4& Color) override;
		virtual void SetClearColor(float R,float G,float B,float A) override;
		virtual void Clear()  override;
		//virtual void DrawIndexed(uint32)  override; // needs to be implemnted
		virtual void SetViewPort(int num,int num2,int Width,int Height) override;
		virtual void EnableDepth(bool Depth) override;
	private:
		Count<VulkanGraphicsContext> Graphics = std::static_pointer_cast<VulkanGraphicsContext>(CurrentWindow::GetGraphicsContext());
	};
}
