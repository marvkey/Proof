#pragma once
#include "Proof/Renderer/RendererAPI.h"
struct GlfwWindow;
namespace Proof {
   class Proof_API OpenGLRendererAPI: public RendererAPI {
   public:
	  virtual void SetClearColor(const glm::vec4& Color) override;
	  virtual void SetClearColor(float R,float G,float B,float A) override;
	  virtual void Clear()  override;

	  virtual void DrawArrays(uint32_t Count) override;
	  virtual void DrawIndexed(const Count<VertexArray>& ArrayObject)override;
	  virtual void DrawIndexed(const Count<VertexArray>& ArrayObject,uint32_t Count) override;

	  virtual void DrawElementIndexed(const Count<VertexArray>& ArrayObject,uint32_t Count,uint32_t AmountElement)override;
	  virtual void DrawElementIndexed(const Count<VertexArray>& ArrayObject,uint32_t AmountElement)override;

	  virtual void SetViewPort(int num,int num2,int Width,int Height) override;
	  virtual void SwapBuffer(GLFWwindow* Window);
	  virtual void EnableDepth(bool Depth);
	  virtual void PollEvents();
   private:
	   bool m_IsDepthEnabled;
   };
}

