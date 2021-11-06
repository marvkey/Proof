#include "Proofprch.h"
#include "OpenGLFrameBuffer.h"

namespace Proof
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(){
		glGenFramebuffers(1,&m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER,m_ID);
	}
	void OpenGLFrameBuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER,m_ID);
	}
	void OpenGLFrameBuffer::UnBind() {
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}
	uint32_t OpenGLFrameBuffer::GetID() {
		return m_ID;
	}
	void OpenGLFrameBuffer::ReadBuffer() {
	}
	void OpenGLFrameBuffer::WriteBuffer(const uint32_t m_FrameBufferID) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER,m_ID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,m_FrameBufferID);
	}
	void OpenGLFrameBuffer::AttachColourTexture(FrameBufferTextureType type,uint32_t colourAttachPosition,uint32_t id,uint32_t level) {
		glBindFramebuffer(GL_FRAMEBUFFER,m_ID);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+ colourAttachPosition,(uint32_t)type,id,level);

	}
	void OpenGLFrameBuffer::AttachOtherTexture(FrameBufferAttachmentType type,FrameBufferTextureType textureType,uint32_t id,uint32_t level) {
		glBindFramebuffer(GL_FRAMEBUFFER,m_ID);
		glFramebufferTexture2D(GL_FRAMEBUFFER,(uint32_t)type,(uint32_t)textureType,id,level);

	}
	void OpenGLFrameBuffer::AttachRenderBuffer(FrameBufferAttachmentType type,uint32_t id) {
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,(uint32_t)type,GL_RENDERBUFFER,id);
	}
}