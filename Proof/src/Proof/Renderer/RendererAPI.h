#pragma once
#include <glm/glm.hpp>
#include"VertexArray.h"
namespace Proof {
	class Proof_API RendererAPI {
	public:
		enum class API {
			None =0, OpenGL =1
		};
		virtual void SetClearColor(const glm::vec4 Color) = 0;
		virtual void SetClearColor(float R, float G, float B, float A)= 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray> VertexArray) = 0;
		inline static API  GetAPI() { return ActiveAPI; }
		virtual void SwapBuffer(GLFWwindow* Window) = 0;
		virtual void EnableDepth() = 0;
		virtual void PollEvents()=0;
	private:
		static API ActiveAPI;
	};
}


