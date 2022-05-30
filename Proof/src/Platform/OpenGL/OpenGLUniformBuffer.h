#pragma once
#include "Proof/Renderer/UniformBuffer.h"
namespace Proof{
	class Proof_API OpenGLUniformBuffer: public UniformBuffer {
	public:
		/*
		* Createas a uniform buffer
		* @param name= the identifier of the uniform buffer
		* @param size = the size of the uniform buffer in bytes
		* @param biningPoint = the binding point of the uniform buffer
		*/ 
		OpenGLUniformBuffer(uint32_t size,uint32_t binding);

		/**
		* changes the data of a set data in the uniform buffer
		* @param Data the location of the variable or variables given an array
		* @param Size of element in bytes
		* @param the offset in the uniform buffer from the stating point in bytes
		*/
		virtual void SetData(void* data, uint32_t size, uint32_t offset=0)override;

		virtual ~OpenGLUniformBuffer();
	private:
		uint32_t m_ID=0;
	};
}
