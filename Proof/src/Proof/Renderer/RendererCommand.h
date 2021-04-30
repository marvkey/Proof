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
		static void EnableDepth();
		inline static void DrawIndexed(const std::shared_ptr<VertexArray> _VertexArray) {
			_RendererAPI->DrawIndexed(_VertexArray);
		}
	private:
		static RendererAPI* _RendererAPI;
	};
}
