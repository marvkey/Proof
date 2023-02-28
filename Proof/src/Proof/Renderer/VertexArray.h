#pragma once
#include "Buffer.h"
namespace Proof {
	enum class VertexInputRate {
		Vertex = 0,
		Instance = 1,
	};
	struct VertexInputDataConfig {
		uint32_t Size;
		VertexInputRate InputRate;
		VertexInputDataConfig(uint32_t size, VertexInputRate inputRate = VertexInputRate::Vertex) :
			Size(size), InputRate(inputRate) {

		}
	};
	class VertexArray {
	public:
		/**
		* create a vertex array
		* @param Size, the size of the vertex array
		*/
		static Count<VertexArray> Create(const std::vector<VertexInputDataConfig>&rate);
		/**
		* Delete the vertex array
		*/
		virtual ~VertexArray() = default;
		/**
		* sets the data of the vertex array
		* @param Positon, the position we want to send to the shader
		* @param the size of component, E.G. Vector will be 3, Float will be 1
		* @param the size of one full vertex
		* @param offset of the first component
		*/
		virtual void AddData(uint32_t Position, DataType Count, size_t Offset, uint32_t binding = 0) = 0;
	};
}