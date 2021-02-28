#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Proof/Core/Window.h"
#include "Proof/Input/Input.h"
#include <vector>
int PROOF_DOUBLE_CLICK = 3;
namespace Proof {
    
    class Proof_API WindowsWindow : public Window {
    public:
        WindowsWindow(unsigned int Width, unsigned int Height);
        friend class Application;
        static std::vector<KeyBoardKey> KeyboardPressed;
        static std::vector<KeyBoardKey> KeyboardReleased;
        static std::vector<KeyBoardKey> KeyboardKeyDoubleClicked;
        static std::vector<KeyBoardKey> KeyboardKeyRepeat;

        static std::vector<MouseButton> MouseButtonPressed;
        static std::vector<MouseButton> MouseButtonReleased;
        static std::vector<MouseButton> MouseButtonDoubleClicked;
        static std::vector<MouseButton> MouseButtonRepeat;
        
    protected:

    private:
        static int CountClicks;
        static double  time;
        virtual	int  createWindow()override;
        void NewFrame();
        virtual void NextFrame(bool UsingGui);
        int WindowEnd();
        unsigned int Width =0, Height=0;
        GLFWwindow* MainWindow;
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        friend class CurrentWindow;
    };
}


