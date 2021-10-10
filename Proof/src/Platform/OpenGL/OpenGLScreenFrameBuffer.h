#pragma once
#include "Proof/Renderer/ScreenFrameBuffer.h"
#include "OpenGLTexture.h"
#include "Proof/Core/Application.h"
namespace Proof {
	class Proof_API OpenGLScreenFrameBuffer: public ScreenFrameBuffer {
	public:
		/**
		* create a new Frame Buffer
		* @param Width, the width of the frame buffer to render
		* @param Height, the Height of the frame buffer to render
		*/
		OpenGLScreenFrameBuffer(uint32_t Width,uint32_t Height);
		/**
		* Deletes the frame buffer
		*/
		virtual ~OpenGLScreenFrameBuffer()override;
		/**
		* Set as current frame buffer
		*/
		virtual void Bind()override;
		/**
		* Remove as current frame buffer
		*/
		virtual void UnBind()override;
		/**
		* changes the size of the frame buffer
		* @param Width, the width of the frame buffer to render
		* @param Height, the Height of the frame buffer to render
		*/
		virtual void Resize(uint32_t Width,uint32_t Height)override;
		/**
		* returns the Texture
		*/
		virtual uint32_t GetTexture() { return m_TextureID; }
		/**
		* returns the FrameBuffer ID
		*/
		virtual uint32_t GetFrameBufferID(){return m_ID;}
	private:
		uint32_t m_TextureID;
		uint32_t m_ID;
		uint32_t m_RenderID;
		/**
		* creates a new Frame Buffer
		*/
		void Instaniate();
		uint32_t m_FrameWidth,m_FrameHeight;
	};
}

