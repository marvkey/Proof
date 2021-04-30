#pragma once
#include "Proof/Renderer/RendererAPI.h"
struct GlfwWindow;
namespace Proof {
	class Proof_API OpenGLRendererAPI : public RendererAPI{
	public:
		virtual void SetClearColor(const glm::vec4 Color) override;
		virtual void SetClearColor(float R, float G, float B, float A) override;
		virtual void Clear()  override;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray> VertexArray)  override; // needs to be implemnted
		void SwapBuffer(GLFWwindow* Window);
		void EnableDepth();
		void PollEvents();
	};
}

