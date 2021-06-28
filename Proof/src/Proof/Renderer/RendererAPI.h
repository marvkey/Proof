#pragma once
#include <glm/glm.hpp>
#include"VertexArray.h"
namespace Proof {
	class Proof_API RendererAPI {
	public:
		enum class API {
			None =0, OpenGL =1, Vulkan=2
		};
		virtual void SetClearColor(const glm::vec4&Color) = 0;
		virtual void SetClearColor(float R, float G, float B, float A)= 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Count<VertexArray>& ArrayObject) = 0;
		virtual void DrawIndexed(const Count<VertexArray>& ArrayObject,uint32_t Count) = 0;
		virtual void DrawElementIndexed(const Count<VertexArray>& ArrayObject,uint32_t Count,uint32_t AmountElement) =0;
		virtual void DrawElementIndexed(const Count<VertexArray>& ArrayObject,uint32_t AmountElement) = 0;
		
		inline static API GetAPI() { return ActiveAPI; }
		virtual void SwapBuffer(GLFWwindow* Window) = 0;
		virtual void EnableDepth(bool Depth) = 0;
		virtual void PollEvents()=0;
		virtual void SetViewPort(int num ,int num2 ,int Width,int Height) = 0;
	private:
		static API ActiveAPI;
	};
}