#include "Proofprch.h"
#include "OpenGLGraphicsContext.h"
#include "GL/glew.h"
namespace Proof {
    OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* _WindowHandle)
        :WindowHandle(_WindowHandle)
    {
        PF_CORE_ASSERT(_WindowHandle, "Window Handle is Null");
    }
    void OpenGLGraphicsContext::Init(){
        glfwMakeContextCurrent(WindowHandle);
        int Status = glewInit();
        PF_CORE_ASSERT(Status, "GLew is not initilized");
    }
    void OpenGLGraphicsContext::SwapBuffer(){
        glfwSwapBuffers(WindowHandle);
    }
}