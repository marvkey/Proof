#pragma once
#include "Proof/Core/Core.h"
#include "Renderer.h"
namespace Proof{
	class SwapChain {
	public:
		static Count<SwapChain> Create(ScreenSize size);
		virtual ScreenSize GetSwapChainExtent()const = 0;
		virtual ScreenSize GetWindowSize()const = 0;
		virtual void WaitAndResetFences(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight)=0;
		virtual void ResetFences(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight)=0;
		virtual void WaitFences(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight) = 0;
		virtual void Resize(ScreenSize size)=0;
		virtual size_t GetImageCount()const = 0;
		virtual Count<class RenderPass> GetRenderPass() = 0;
		template<class T>
		T* As() {
			return  dynamic_cast<T*>(this);
		}
	};
}
