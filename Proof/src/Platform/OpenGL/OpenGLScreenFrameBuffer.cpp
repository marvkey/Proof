#include "Proofprch.h"
#include "OpenGLScreenFrameBuffer.h"
#include "glad/glad.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Proof/Core/Application.h"
namespace Proof {
	OpenGLScreenFrameBuffer::OpenGLScreenFrameBuffer(uint32_t Width,uint32_t Height)
		:m_FrameWidth(Width), 
		m_FrameHeight(Height)
	{
		Instaniate();
		bool m_BufferHasBeenCreated = true;
	}
	OpenGLScreenFrameBuffer::~OpenGLScreenFrameBuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		glDeleteFramebuffers(1,&m_ID);
	}
	void OpenGLScreenFrameBuffer::Bind(){
		glBindFramebuffer(GL_FRAMEBUFFER,m_ID);
		glViewport(0,0,m_FrameWidth,m_FrameHeight);
	}
	void OpenGLScreenFrameBuffer::UnBind() {
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}
	void OpenGLScreenFrameBuffer::Resize(uint32_t Width,uint32_t Height){
		m_FrameWidth = Width;
		m_FrameHeight = Height;
		Instaniate();
	}
	void OpenGLScreenFrameBuffer::WriteBuffer(const uint32_t m_FrameBufferID) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER,m_ID);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,m_FrameBufferID);
	}

	void OpenGLScreenFrameBuffer::Instaniate() {
		if (m_BufferHasBeenCreated == true) {
			glBindFramebuffer(GL_FRAMEBUFFER,0);
			glDeleteFramebuffers(1, &m_ID);
			glDeleteTextures(1, &m_ID);
			glDeleteRenderbuffers(1, &m_ID);
		}
		glGenFramebuffers(1,&m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER,m_ID);

		glGenTextures(1,&m_TextureID);
		glBindTexture(GL_TEXTURE_2D,m_TextureID);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,m_FrameWidth,m_FrameHeight,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,m_TextureID,0);

		glGenRenderbuffers(1,&m_RenderID);
		glBindRenderbuffer(GL_RENDERBUFFER,m_RenderID);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,m_FrameWidth,m_FrameHeight); 
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,m_RenderID);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			PF_ENGINE_ERROR("Screen Frame Buffer is not complete");
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}
}