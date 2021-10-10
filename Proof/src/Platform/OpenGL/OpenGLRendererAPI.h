#pragma once
#include "Proof/Renderer/RendererAPI.h"
struct GlfwWindow;
namespace Proof {
   class Proof_API OpenGLRendererAPI: public RendererAPI {
   public:
	  virtual void SetClearColor(const glm::vec4& Color) override;
	  virtual void SetClearColor(float R,float G,float B,float A) override;
	  virtual void DrawArrays(uint32_t Count) override;
	  virtual void DrawIndexed(const Count<VertexArray>& ArrayObject)override;
	  virtual void DrawIndexed(const Count<VertexArray>& ArrayObject,uint32_t Count) override;

	  virtual void DrawElementIndexed(const Count<VertexArray>& ArrayObject,uint32_t Count,uint32_t AmountElement)override;
	  virtual void DrawElementIndexed(const Count<VertexArray>& ArrayObject,uint32_t AmountElement)override;

	  virtual void SetViewPort(uint32_t width,uint32_t height) override;
	  virtual void SwapBuffer(GLFWwindow* Window);
	  virtual void Enable(uint32_t bitField);
	  virtual void Disable(uint32_t bitField);
	  virtual void DepthFunc(DepthType type);
	  virtual void PollEvents();
	  virtual void Clear(uint32_t bitField);
   };
}

