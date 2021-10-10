#pragma once
#include <glm/glm.hpp>
#include "RendererAPI.h"

struct ::GLFWwindow;
namespace Proof {
	class Proof_API RendererCommand {
	public:
		static void PollEvents();
		static void SetClearColor(glm::vec4 Color);
		static void SetClearColor(float R, float G, float B, float A);
		static void SwapBuffer(GLFWwindow* window);
		static void SetViewPort(uint32_t width,uint32_t height);
		inline static void DrawIndexed(const Count<VertexArray>& ArrayObject) {
			s_RendererAPI->DrawIndexed(ArrayObject);
		}
		inline static void DrawIndexed(const Count<VertexArray>& ArrayObject,uint32_t Count){
			s_RendererAPI->DrawIndexed(ArrayObject,Count);
		}

		static void DrawElementIndexed(const Count<VertexArray>& ArrayObject,uint32_t Count,uint32_t AmountElement){
			s_RendererAPI->DrawElementIndexed(ArrayObject,Count,AmountElement);
		}
		static void DrawElementIndexed(const Count<VertexArray>& ArrayObject,uint32_t AmountElement){
			s_RendererAPI->DrawElementIndexed(ArrayObject,AmountElement);
		}

		inline static void DrawArray(uint32_t Count){
			s_RendererAPI->DrawArrays(Count);
		}
		/**
		* @param bitfield use the enum form the struct Proof Clear
		*/
		inline static void Clear(uint32_t bitField){
			s_RendererAPI->Clear(bitField);
		}
		/**
		* @param bitfield use the enum form the struct ProofRenderTest
		*/
		inline static void Enable(uint32_t bitField){
			s_RendererAPI->Enable(bitField);
		}
		/**
		* @param bitfield use the enum form the struct ProofRenderTest
		*/
		inline static void Disable(uint32_t bitField){
			s_RendererAPI->Disable(bitField);
		}

		inline static void DepthFunc(DepthType type){
			s_RendererAPI->DepthFunc(type);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}
