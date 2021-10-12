#include "Proof/Renderer/GraphicsContext.h"
struct GLFWwindow;
namespace Proof
{
	class Proof_API OpenGLGraphicsContext: public GraphicsContext {
	public:
		OpenGLGraphicsContext(GLFWwindow* _WindowHandle);
		virtual void Init()override;
		virtual void SwapBuffer() override;
		virtual void CleanUp() override;
	private:
		GLFWwindow* WindowHandle;
		static void OpenGLMessageCallback(
		  unsigned source,
		  unsigned type,
		  unsigned id,
		  unsigned severity,
		  int length,
		  const char* message,
		  const void* userParam);
	};
}