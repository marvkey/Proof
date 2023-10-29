#pragma once
#include "Proof/Core/Core.h"
#include "Texture.h"
#include <glm/glm.hpp>
namespace Proof{

	struct RendererConfig;

	class SwapChain : public RefCounted {
	public:
		static Count<SwapChain> Create(const class Window* window);

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual glm::uvec2 GetSize() = 0;
		virtual void Resize(glm::uvec2 size)=0;
		virtual size_t GetImageCount()const = 0;

		virtual ImageFormat GetColorFormat() = 0;
		virtual ImageFormat GetDepthFormat() = 0;

		virtual RendererConfig GetRenderConfig() = 0;
		virtual uint32_t GetFrameIndex() = 0;
		virtual uint32_t GetImageIndex() = 0;

		virtual void SetVsync(bool vsync) = 0;
		virtual bool GetVsync() = 0;
		//virtual ImageLayouts2D GetImageLayout() = 0;
		//template<class T>
		//T* As() {
		//	return  dynamic_cast<T*>(this);
		//}
	};
}
