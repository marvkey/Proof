#include "Proofprch.h"
#include "OpenGLGraphicsContext.h"
//#include <Glad/glad.h>
#include <GLFW/glfw3.h>
#include "Proof/Renderer/Renderer.h"
namespace Proof
{
    OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* _WindowHandle)
        :WindowHandle(_WindowHandle)    {
        PF_CORE_ASSERT(_WindowHandle,"Window Handle is Null");
    }
    void OpenGLGraphicsContext::Init() {
        glfwMakeContextCurrent(WindowHandle);
        int Status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        PF_CORE_ASSERT(Status,"GLew is not initilized");
        Renderer::s_RenderCompany = (const char*)glGetString(GL_VENDOR);
        Renderer::s_GraphicsCard = (const char*)glGetString(GL_RENDERER);
        Renderer::s_GraphicsCardVersion = (const char*)glGetString(GL_VERSION);

        PF_ENGINE_INFO("Vendor: %s",glGetString(GL_VENDOR));
        PF_ENGINE_INFO("Renderer: %s",glGetString(GL_RENDERER));
        PF_ENGINE_INFO("Version: %s",glGetString(GL_VERSION));
        PF_CORE_ASSERT((GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5)),"Proof requires at least OpenGL version 4.5!");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    #ifdef PF_ENABLE_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGLMessageCallback,nullptr);

        glDebugMessageControl(GL_DONT_CARE,GL_DONT_CARE,GL_DEBUG_SEVERITY_NOTIFICATION,0,NULL,GL_FALSE);
    #endif
    }
    void OpenGLGraphicsContext::SwapBuffer() {
        glfwSwapBuffers(WindowHandle);
    }
    void OpenGLGraphicsContext::CleanUp() {}
    void OpenGLGraphicsContext::OpenGLMessageCallback(unsigned source,unsigned type,unsigned id,unsigned severity,int length,const char* message,const void* userParam) {
        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         PF_ENGINE_CRITICAL(message); return;
        case GL_DEBUG_SEVERITY_MEDIUM:       PF_ENGINE_INFO(message); return;
        case GL_DEBUG_SEVERITY_LOW:          PF_ENGINE_WARN(message); return;
        case GL_DEBUG_SEVERITY_NOTIFICATION: PF_ENGINE_TRACE(message); return;
        }

        PF_CORE_ASSERT(false,"Open GL Unknown severity level!");
    }
}