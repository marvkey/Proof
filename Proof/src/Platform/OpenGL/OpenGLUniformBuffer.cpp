#include "Proofprch.h"
#include "OpenGLUniformBuffer.h"
#include "Glad/glad.h"

namespace Proof {
	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size,uint32_t binding) {
		glCreateBuffers(1,&m_ID);
		glNamedBufferData(m_ID,size,nullptr,GL_DYNAMIC_DRAW); 
		glBindBufferBase(GL_UNIFORM_BUFFER,binding,m_ID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer() {
		glDeleteBuffers(1,&m_ID);
	}


	void OpenGLUniformBuffer::SetData(const void* data,uint32_t size,uint32_t offset) {
		glNamedBufferSubData(m_ID,offset,size,data);
	}
}