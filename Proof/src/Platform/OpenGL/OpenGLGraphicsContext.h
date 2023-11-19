#include "Proof/Renderer/GraphicsContext.h"
namespace Proof
{
#if 0
	class Proof_API OpenGLGraphicsContext: public GraphicsContext {
	public:
		OpenGLGraphicsContext(Window* windowHandle);
		~OpenGLGraphicsContext(){
		}
	private:
		virtual void Init();
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
#endif
}