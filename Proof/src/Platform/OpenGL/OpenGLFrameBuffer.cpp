#include "Proofprch.h"
#include "OpenGLFrameBuffer.h"
#include"glad/glad.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Proof/Core/Application.h"
namespace Proof {
	OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t Width,uint32_t Height)
		:FrameWidth(Width), FrameHeight(Height)
	{
		Instaniate();
	}
	OpenGLFrameBuffer::~OpenGLFrameBuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		glDeleteFramebuffers(1,&m_ID);
	}
	void OpenGLFrameBuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER,m_ID);
		glViewport(0,0,FrameWidth,FrameHeight);
	}
	void OpenGLFrameBuffer::UnBind() {
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}


	void OpenGLFrameBuffer::Resize(uint32_t Width,uint32_t Height){
		glDeleteFramebuffers(1,&m_ID);
		glDeleteTextures(1,&m_ID);
		glDeleteRenderbuffers(1,&m_ID);
		FrameWidth = Width;
		FrameHeight = Height;
		Instaniate();
	}

	void OpenGLFrameBuffer::Instaniate() {
		glGenFramebuffers(1,&m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER,m_ID);

		glGenTextures(1,&Texture_ID);
		glBindTexture(GL_TEXTURE_2D,Texture_ID);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,FrameWidth,FrameHeight,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,Texture_ID,0);

		glGenRenderbuffers(1,&Render_ID);
		glBindRenderbuffer(GL_RENDERBUFFER,Render_ID);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,FrameWidth,FrameHeight); 
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,Render_ID);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			PF_ENGINE_ERROR("frame Buffer is not complete ");
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}
	
}