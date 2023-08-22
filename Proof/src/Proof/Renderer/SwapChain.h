#pragma once
#include "Proof/Core/Core.h"
#include "Texture.h"
namespace Proof{
	class SwapChain : public RefCounted {
	public:
		static Count<SwapChain> Create(ScreenSize size, bool vsync);
		virtual ScreenSize GetSwapChainExtent()const = 0;
		virtual ScreenSize GetWindowSize()const = 0;
		virtual void WaitAndResetFences(uint32_t frameInfllight)=0;
		virtual void ResetFences(uint32_t frameInfllight)=0;
		virtual void WaitFences(uint32_t frameInfllight) = 0;
		virtual void Resize(ScreenSize size)=0;
		virtual size_t GetImageCount()const = 0;

		virtual ImageFormat GetImageFormat() = 0;
		virtual ImageFormat GetDepthFormat() = 0;
		virtual void SetVsync(bool vsync) = 0;
		virtual bool GetVsync() = 0;
		virtual ImageLayouts2D GetImageLayout() = 0;
		//template<class T>
		//T* As() {
		//	return  dynamic_cast<T*>(this);
		//}
	};
}
