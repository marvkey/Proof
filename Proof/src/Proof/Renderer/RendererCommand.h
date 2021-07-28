#pragma once
#include <glm/glm.hpp>
#include "RendererAPI.h"

struct ::GLFWwindow;
namespace Proof {
	class Proof_API RendererCommand {
	public:
		static void PollEvents();
		static void Clear();
		static void SetClearColor(glm::vec4 Color);
		static void SetClearColor(float R, float G, float B, float A);
		static void SwapBuffer(GLFWwindow* window);
		static void EnableDepth(bool TrueOrFalse);
		static void SetViewPort(int Width,int Height);
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
	private:
		static RendererAPI* s_RendererAPI;
	};
}
