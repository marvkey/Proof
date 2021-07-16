#pragma once
#include "Proof/Core/Core.h"
#include "Texture.h"
#include"Buffer.h"
namespace Proof {
	class Proof_API FrameBuffer {
	public:
		/**
		* Set as current frame buffer
		*/
		virtual void Bind() = 0;
		/**
		* Remove as current frame buffer
		*/
		virtual void UnBind() = 0;
		/**
		* changes the size of the frame buffer
		* @param Width, the width of the frame buffer to render
		* @param Height, the Height of the frame buffer to render
		*/
		virtual void Resize(uint32_t Width,uint32_t Height)= 0;
		/**
		* Deletes the frame buffer
		*/
		virtual ~FrameBuffer() = default;
		/**
		* creates a new Frame Buffer
		* @param Width, the width of the frame buffer to render
		* @param Height, the Height of the frame buffer to render
		*/
		static Count<FrameBuffer> Create(uint32_t Width,uint32_t Height);
		/**
		* returns the Texture ID
		*/
		virtual uint32_t GetTexture() = 0;

		/**
		* returns the FrameBuffer ID
		*/
		virtual uint32_t GetFrameBufferID() = 0;
	};
}

