#include "Proof/Renderer/GraphicsContext.h"
namespace Proof
{
	class Proof_API OpenGLGraphicsContext: public GraphicsContext {
	public:
		OpenGLGraphicsContext(Window* windowHandle);
		virtual void Init()override;
		virtual void SwapBuffer() override;
		virtual void CleanUp() override;
	private:
		Window* WindowHandle;
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