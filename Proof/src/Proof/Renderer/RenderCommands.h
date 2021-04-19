#pragma once
#include <glm/glm.hpp>
struct GLFWwindow;
namespace Proof {
	class Proof_API RenderCommands {
	public:
		static void PollEvents();
		static void ClearBuffer();
		static void SetColor(glm::vec4 Color);
		static void SetColor(float R, float G, float B, float A);
		static void SwapBuffer(GLFWwindow* window);
	};
}
