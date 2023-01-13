#pragma once
#include "Proof/Core/Core.h"
#include "Texture.h"
#include"Buffer.h"
namespace Proof {
	class Proof_API ScreenFrameBuffer {
	public:
		template<class T>
		T* As() {
			return  dynamic_cast<T*>(this);
		}
		/**
		* Set as current frame buffer
		*/
		virtual void Bind(){};
		/**
		* Remove as current frame buffer
		*/
		virtual void UnBind(){};
		/**
		* changes the size of the frame buffer
		* @param Width, the width of the frame buffer to render
		* @param Height, the Height of the frame buffer to render
		*/
		virtual void Resize(uint32_t Width,uint32_t Height, Count<class RenderPass> renderPass = nullptr)= 0;
		virtual void Resize(Vector2 imageSize, Count<class RenderPass> renderPass = nullptr) = 0;

		/**
		* Deletes the frame buffer
		*/
		virtual ~ScreenFrameBuffer() = default;
		/**
		* creates a new Frame Buffer
		* @param Width, the width of the frame buffer to render
		* @param Height, the Height of the frame buffer to render
		*/
		static Count<ScreenFrameBuffer> Create(uint32_t Width,uint32_t Height);
		static Count<ScreenFrameBuffer> Create(Vector2 imageSize, Count<class RenderPass> renderPass = nullptr, bool screenPresent = false);
		virtual Image GetImage() = 0;
		virtual uint32_t GetFrameWidth() = 0;
		virtual uint32_t GetFrameHeight() = 0;
	private:
		
	};
}

