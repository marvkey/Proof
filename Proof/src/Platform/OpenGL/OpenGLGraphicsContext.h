#include "Proof/Renderer/GraphicsContext.h"
namespace Proof
{
	class Proof_API OpenGLGraphicsContext: public GraphicsContext {
	public:
		OpenGLGraphicsContext(Window* windowHandle);
		~OpenGLGraphicsContext(){
			CleanUp() ;
		}
		virtual void SwapBuffer() override;
	private:
		virtual void CleanUp() override;
		virtual void Init()override;
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