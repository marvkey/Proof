#pragma once
#include "Proof/Core/Core.h"
namespace Proof
{
	class Proof_API UniformBuffer {
	public:
		/*
		* Createas a uniform buffer
		* @param name= the identifier of the uniform buffer
		* @param size = the size of the uniform buffer in bytes
		* @param biningPoint = the binding point of the uniform buffer
		*/
		static Count<UniformBuffer>Create(uint32_t size,uint32_t binding);

		/**
		* changes the data of a set data in the uniform buffer
		* @param Data the location of the variable or variables given an array
		* @param Size of element in bytes
		* @param the offset in the uniform buffer from the stating point in bytes if it is the last element just use the current size of the uniform buffer
		*/
		virtual void SetData(void* data, uint32_t size, uint32_t offset = 0) = 0;
		template<typename T>
		T* As() {
			return dynamic_cast<T*>(this);
		}
		virtual ~UniformBuffer(){};
		/*
		virtual const uint32_t GetBindingPoint() = 0;

		virtual const uint32_t GetID() = 0;
		virtual std::string GetName() = 0;

		virtual const uint32_t GetMaxSize() = 0;
		*/
	};
}
