#pragma once
#include "Proof/Renderer/FrameBuffer.h"
namespace Proof
{
	class OpenGLFrameBuffer: public FrameBuffer  
	{
	public:
		OpenGLFrameBuffer();
		virtual void Bind();
		virtual void UnBind();
		virtual uint32_t GetID();
		virtual void ReadBuffer();
		virtual void WriteBuffer(const uint32_t m_FrameBufferID);
		virtual void AttachColourTexture(FrameBufferTextureType type,uint32_t colourAttachPosition,uint32_t id,uint32_t level =0 );
		virtual void AttachOtherTexture(FrameBufferAttachmentType type,FrameBufferTextureType textureType,uint32_t id,uint32_t level=0);
		virtual void AttachRenderBuffer(FrameBufferAttachmentType type,uint32_t id);
	private:
		uint32_t m_ID;
	};
}
