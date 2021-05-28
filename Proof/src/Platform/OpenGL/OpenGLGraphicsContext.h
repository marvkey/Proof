#include "Proof/Renderer/GraphicsContext.h"
#include "Proof/Core/Core.h"
class GLFWwindow;
namespace Proof {
   class Proof_API OpenGLGraphicsContext : public GraphicsContext {
   public:
      OpenGLGraphicsContext (GLFWwindow* _WindowHandle);
      virtual void Init ()override;
      virtual void SwapBuffer () override;
   private:
      GLFWwindow* WindowHandle;
   };
}