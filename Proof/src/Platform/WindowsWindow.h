#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Proof/Core/Window.h"
#include "Proof/Core/GUI.h"


namespace Proof {

    class Proof_API WindowsWindow : public Window {
    public:
        WindowsWindow(unsigned int Width, unsigned int Height);

        friend class Application;

        virtual GLFWwindow* GetWindow();
        virtual unsigned int GetWidth();
        virtual unsigned int GetHeight();

        void SetWindowSize(unsigned int width, unsigned int height);
    protected:
        virtual void OnNewFrame();
    private:
        virtual	int  createWindow()override;

        void WindowBegin();
        void windowPollEvents(bool UsingGui);
        int  WindowEnd();
        

        unsigned int Width =0, Height=0;
        GLFWwindow* MainWindow;
        std::pair<float, float> GetPlatformMouseLocation();
        std::pair<int, int> GetWindowSize();
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    };
}


