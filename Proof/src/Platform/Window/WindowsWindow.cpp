#include "Proofprch.h"
#include "WindowsWindow.h"
#include "Proof/Input/KeyCodes.h"
#include "Proof/Events/MouseEvent.h"
#include  "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Events/ControllerEvent.h"

#include "Proof/Core/FrameTime.h"

#include "Proof/Renderer/GraphicsContext.h"
#include "Proof/Renderer/SwapChain.h"
#include "Proof/Core/Core.h"
#include <GLFW/glfw3.h>
#include <Glad/glad.h>

#include "Proof/Renderer/Renderer.h"
namespace Proof {

    static uint8_t s_GLFWWindowCount = 0;
    static void GLFWErrorCallback(int error, const char* description)
    {
        PF_ENGINE_ERROR("GLFW {}: {}", error, description);
    }
    WindowsWindow::WindowsWindow(const WindowConfiguration& configuration) :
        Window(configuration)
    {
        Init();
        s_lastWindow = this;

        int gamepadCount = 0;
        GLFWgamepadstate gamepadState;
        while (glfwJoystickPresent(gamepadCount))
        {
            ControllerCallbackConnect(gamepadCount, GLFW_CONNECTED);
            gamepadCount++;
        }

    }
    WindowsWindow::~WindowsWindow()
    {
        End();
    }
    void WindowsWindow::WindowUpdate() {

        KeyboardClicked.clear();
        KeyboardReleased.clear();
        KeyboardKeyDoubleClicked.clear();
        KeyboardKeyHold.clear();

        MouseButtonClicked.clear();
        MouseButtonReleased.clear();
        MouseButtonDoubleClicked.clear();
        MouseButtonReleased.clear();

        MouseScrollX.clear();
        MouseScrollY.clear();
        ControllerEventCallback();
        //HAVE TO DO THIS SINCE SOMETIEMS GLFW DOES NOT RECIEVE THE KEY RELEASED DUE TO SO MANY KEYS 
        // BEING PRESSED AT THE SAME TIME
        // WE ARE BASICALLY GOING TO DO THIS EVENT FOR THE INPUT MANAGER AXIS


        // Checking to see if every key clicked in the last frame is still clicked 
        // if teh key is not pressed we remove it from the list, set it false in the key pressed list
        {
            for (int i = 0; i < m_KeyPressedEventCheck.size(); i++)
            {
                KeyBoardKey key = m_KeyPressedEventCheck[i];
                if (glfwGetKey((GLFWwindow*)m_Window, (int)key))
                {
                    KeyPressedEvent pressedEvent(key);
                    EventCallback(pressedEvent);
                }
                else
                {
                    KeyPressed[(int)key] = false;
                    m_KeyPressedEventCheck.erase(m_KeyPressedEventCheck.begin() + i);
                }
            }

            for (int i = 0; i < m_MouseButtonPressedEventCheck.size(); i++)
            {
                MouseButton key = m_MouseButtonPressedEventCheck[i];
                if (glfwGetMouseButton((GLFWwindow*)m_Window, (int)key))
                {
                    MouseButtonPressedEvent pressedEvent(key);
                    EventCallback(pressedEvent);
                }
                else
                {
                    MouseButtonPressed[(int)key] = false;
                    m_MouseButtonPressedEventCheck.erase(m_MouseButtonPressedEventCheck.begin() + i);
                }
            }
        }
        glfwSwapBuffers((GLFWwindow*)m_Window);
        glfwPollEvents();
        m_FrameBufferResized = false;
        m_MouseMoved = false;
    }
    void WindowsWindow::key_callback(int key, int scancode, int action, int mods) {
        /* This is for when a key is Clicked 2 */
        if (action == GLFW_RELEASE)
        {
            static auto before = std::chrono::system_clock::now();
            auto now = std::chrono::system_clock::now();
            double diff_ms = std::chrono::duration <double, std::milli>(now - before).count();
            before = now;
            if (diff_ms > 10 && diff_ms < 200)
            {
                action = (int)InputEvent::KeyDouble;
            }
        }

        switch (action)
        {
            case GLFW_PRESS:
                KeyboardClicked.emplace_back((KeyBoardKey)key);
                {
                    // have to use this because GLFW someties will not send that a key hasb een released

                    KeyClickedEvent keyevent((KeyBoardKey)key);
                    EventCallback(keyevent);

                    if (key < KeyPressed.size())
                    {
                        KeyPressed[key] = true;
                        m_KeyPressedEventCheck.emplace_back((KeyBoardKey)key);
                    }
                }
                break;
            case GLFW_RELEASE:
                KeyboardReleased.emplace_back((KeyBoardKey)key);
                {
                    // have to use this because GLFW someties will not send that a key hasb een released
                    KeyReleasedEvent keyevent((KeyBoardKey)key);
                    EventCallback(keyevent);
                    if (key < KeyPressed.size())
                    {
                        KeyPressed[key] = false;
                    }
                }
                break;
            case (int)InputEvent::KeyDouble:
                KeyboardKeyDoubleClicked.emplace_back((KeyBoardKey)key);
                {
                    KeyDoubleClickEvent keyevent((KeyBoardKey)key);
                    EventCallback(keyevent);
                }
                break;
            case (int)InputEvent::KeyHold:
                KeyboardKeyHold.emplace_back((KeyBoardKey)key);
                {
                    KeyHoldEvent keyevent((KeyBoardKey)key);
                    EventCallback(keyevent);
                }
                break;
        }

    }

    void WindowsWindow::mouse_button_callback(int button, int action, int mods) {
        if (action == GLFW_RELEASE)
        {
            static auto before = std::chrono::system_clock::now();
            auto now = std::chrono::system_clock::now();
            double diff_ms = std::chrono::duration <double, std::milli>(now - before).count();
            before = now;
            if (diff_ms > 10 && diff_ms < 200)
            {
                action = (int)InputEvent::KeyDouble;
            }
        }
        switch (action)
        {
            case GLFW_PRESS:
                {
                    MouseButtonClicked.emplace_back((MouseButton)button);
                    {
                        m_MouseButtonPressedEventCheck.emplace_back((MouseButton)button);
                        MouseButtonPressed[button] = true;
                        MouseButtonClickedEvent mouseEvent((MouseButton)button);
                        EventCallback(mouseEvent);
                    }
                    break;
                }
            case GLFW_RELEASE:
                MouseButtonReleased.emplace_back((MouseButton)button);
                {
                    MouseButtonReleasedEvent mouseEvent((MouseButton)button);
                    EventCallback(mouseEvent);
                    MouseButtonPressed[button] = false;
                }
                break;
            case (int)InputEvent::KeyDouble:
                MouseButtonDoubleClicked.emplace_back((MouseButton)button);
                {
                    MouseButtonDoubleClickEvent mouseEvent((MouseButton)button);
                    EventCallback(mouseEvent);
                }
                break;
        }
    }

    void WindowsWindow::Window_Resize_Callback(int width, int height) {
        WindowResizeEvent windowResizeEvent(width, height);
        EventCallback(windowResizeEvent);
        if (width == 0 || height == 0)
        {
            WindowMinimizeEvent Minimizedevent(true);
            EventCallback(Minimizedevent);

        }
        else if (m_WindowConfiguration.Width == 0 || m_WindowConfiguration.Height == 0)
        {
            WindowMinimizeEvent Minimizedevent(false);
            EventCallback(Minimizedevent);
        }
        m_WindowConfiguration.Width = width;
        m_WindowConfiguration.Height = height;
    }

    void WindowsWindow::Window_Position_Callback(int xpos, int ypos) {
        WindowMoveEvent windowEvent(xpos, ypos);
        EventCallback(windowEvent);
    }

    void WindowsWindow::Window_Close_Callback() {
        WindowCloseEvent windowEvent(true);
        EventCallback(windowEvent);
    }

    void WindowsWindow::ControllerCallbackConnect(int jid, int event) {

        if (event == GLFW_CONNECTED) // because when we callback when removed it is removed so checking if gamepad to glfw the controller is alreayd removed
            if (glfwJoystickIsGamepad(jid) == false)return; // if we are using a controller


        if (event == GLFW_CONNECTED)
        {
            Controller controller;
            controller.ID = jid;
            controller.Name = glfwGetGamepadName(jid);

            s_Controllers.insert({ jid,  controller });
            ControllerConnectEvent ctEvent(jid);
            s_lastWindow->EventCallback(ctEvent);
        }
        else if (event == GLFW_DISCONNECTED)
        {

            if (!s_Controllers.contains(jid))
                return;

            ControllerDisconnectEvent ctEvent(jid);
            s_lastWindow->EventCallback(ctEvent);

            s_Controllers.erase(jid);
            s_ControllerLastKeyClicked.erase(jid);
            if (s_Controllers.size() == 0)
                return;

            startLoop:
            for (auto& [ID, controller] : s_Controllers)
            {
                if (ID < jid)
                    continue;

                // copy from other controller just change these atributes
                Controller newControll(controller);
                newControll.ID = jid - 1;
                newControll.Name = glfwGetGamepadName(jid - 1);


                // this is for the glfw suport of controllers
                // eras this current controller since we have to change the value of settings 
                s_Controllers.erase(ID);
                // creaet a new contorller with id 
                s_Controllers.insert({ jid - 1,  newControll });

                goto startLoop;
            }

        }
    }

    void WindowsWindow::Window_Input_Focus_callback(int focused) {
        WindowFocusEvent windowEvent(true);
        EventCallback(windowEvent);
    }

    void WindowsWindow::ControllerEventCallback() {
        for (auto& [ID, controller] : s_Controllers)
        {
            ContollerButtonCallback(controller);
            ContollerAxisCallback(controller);
        }
    }

    InputEvent WindowsWindow::ControllerGetState(Controller& controller, GLFWgamepadstate& state, ControllerButton button) {
        if (!s_ControllerLastKeyClicked.contains(controller.ID))
        {
            s_ControllerLastKeyClicked[controller.ID] = ControllerButton::None;
        }
        if ((button == ControllerButton::ButtonLeftTrigger) || (button == ControllerButton::ButtonRightTrigger))
            goto trigerCheck;
        {
            int buttonIndex = (int)button;

            if (state.buttons[buttonIndex] == GLFW_PRESS)
            {
                if (controller.Buttons[button] == InputEvent::KeyPressed || controller.Buttons[button] == InputEvent::KeyClicked) // THE KEY WAS ALSO PRESSED last FRAME
                    return InputEvent::KeyPressed;
                if (controller.Buttons[button] == InputEvent::None)
                { // THE KEY WAS NOT PRESSED LAST FRAME
                    auto it = s_ControllerLastKeyClicked.find(controller.ID);
                    if (it != s_ControllerLastKeyClicked.end())
                    {
                        it->second = (ControllerButton)buttonIndex;
                    }
                    else
                    {
                        s_ControllerLastKeyClicked.insert({ controller.ID,(ControllerButton)buttonIndex });
                    }
                    return InputEvent::KeyClicked;
                }
            }
            if (state.buttons[buttonIndex] == GLFW_RELEASE)
            {
                if (buttonIndex == (int)s_ControllerLastKeyClicked.at(controller.ID))
                { // double click checking
                    if (controller.Buttons[button] == InputEvent::KeyClicked || controller.Buttons[button] == InputEvent::KeyPressed)
                    {
                        static auto before = std::chrono::system_clock::now();
                        auto now = std::chrono::system_clock::now();
                        double diff_ms = std::chrono::duration <double, std::milli>(now - before).count();
                        before = now;

                        if (diff_ms > 10 && diff_ms < 400)
                        {
                            s_ControllerLastKeyClicked.at(controller.ID) = (ControllerButton)-1;
                            return InputEvent::KeyDouble;
                        }
                    }
                }
                // if not double click that means key is released
                if (controller.Buttons[button] == InputEvent::KeyClicked || controller.Buttons[button] == InputEvent::KeyPressed)
                { // THE button WAS ALSO PRESSED last FRAME
                    return InputEvent::KeyReleased;
                }
            }
            return InputEvent::None;
        }

        trigerCheck:
            // TRIGGER INPUT
        {
            // LEFTTRIGGER
            {
                if (button == ControllerButton::ButtonLeftTrigger)
                {
                    if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > 0)
                    { 
                        if (controller.LeftTrigger.State == InputEvent::KeyPressed || controller.LeftTrigger.State == InputEvent::KeyClicked)
                            return InputEvent::KeyPressed;
                        // if the key is pressed
                        if (controller.LeftTrigger.State == InputEvent::None)
                        { // THE KEY WAS NOT PRESSED LAST FRAME
                            if (s_ControllerLastKeyClicked.contains(controller.ID))
                                s_ControllerLastKeyClicked[controller.ID] = ControllerButton::ButtonLeftTrigger;
                            else
                                s_ControllerLastKeyClicked.insert({ controller.ID,ControllerButton::ButtonLeftTrigger });

                            return InputEvent::KeyClicked;
                        }
                        return InputEvent::KeyPressed;
                    }
                    if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] < 0)
                    { // key is released
                        
                        if (ControllerButton::ButtonLeftTrigger == s_ControllerLastKeyClicked.at(controller.ID))
                        { // double click checking
                            if (controller.LeftTrigger.State == InputEvent::KeyClicked || controller.LeftTrigger.State == InputEvent::KeyPressed)
                            { // double click checking
                                static auto before = std::chrono::system_clock::now();
                                auto now = std::chrono::system_clock::now();
                                double diff_ms = std::chrono::duration <double, std::milli>(now - before).count();
                                before = now;

                                if (diff_ms > 10 && diff_ms < 400)
                                {
                                    s_ControllerLastKeyClicked.at(controller.ID) = (ControllerButton)-1;
                                    return InputEvent::KeyDouble;
                                }
                            }
                        }
                        if (controller.LeftTrigger.State == InputEvent::KeyClicked || controller.LeftTrigger.State == InputEvent::KeyPressed)
                        { // THE button WAS ALSO PRESSED last FRAME
                            return InputEvent::KeyReleased;
                        }
                    }
                    return InputEvent::None; // left trigger non
                }
            }
            // RIGHT TRIGGER
            {
                if (button == ControllerButton::ButtonRightTrigger)
                {
                    if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > 0)
                    { // if the key is pressed
                        if (controller.RightTrigger.State == InputEvent::KeyPressed || controller.RightTrigger.State == InputEvent::KeyClicked)
                            return InputEvent::KeyPressed;
                        if (controller.RightTrigger.State == InputEvent::None)
                        { // THE KEY WAS NOT PRESSED LAST FRAME
                            auto it = s_ControllerLastKeyClicked.find(controller.ID);
                            if (it != s_ControllerLastKeyClicked.end())
                            {
                                it->second = ControllerButton::ButtonRightTrigger;
                            }
                            else
                            {
                                s_ControllerLastKeyClicked.insert({ controller.ID,ControllerButton::ButtonRightTrigger });
                            }
                            return InputEvent::KeyClicked;
                        }

                    }
                    if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] < 0)
                    { // key is released
                        if (ControllerButton::ButtonRightTrigger == s_ControllerLastKeyClicked.at(controller.ID))
                        { // double click checking
                            if (controller.RightTrigger.State == InputEvent::KeyClicked || controller.RightTrigger.State == InputEvent::KeyPressed)
                            { // double click checking
                                static auto before = std::chrono::system_clock::now();
                                auto now = std::chrono::system_clock::now();
                                double diff_ms = std::chrono::duration <double, std::milli>(now - before).count();
                                before = now;

                                if (diff_ms > 10 && diff_ms < 400)
                                {
                                    s_ControllerLastKeyClicked.at(controller.ID) = ControllerButton::None;
                                    return InputEvent::KeyDouble;
                                }
                            }
                        }
                        if (controller.RightTrigger.State == InputEvent::KeyClicked || controller.RightTrigger.State == InputEvent::KeyPressed)
                        { // THE button WAS ALSO PRESSED last FRAME
                            return InputEvent::KeyReleased;
                        }
                    }
                    return InputEvent::None; // right trigger non
                }
            }

        }
        return InputEvent::None; // incase
    }
    void WindowsWindow::ContollerButtonCallback(Controller& controller) {
        GLFWgamepadstate state;

        if (glfwGetGamepadState(controller.ID, &state) == false)
            return;
        int ctrlID = controller.ID;
        for (auto& [button, event] : controller.Buttons)
        { // +2 for the trigger kerys4
            InputEvent action = ControllerGetState(controller, state, button);
            controller.Buttons[button] = action;

            switch (action)
            {
                case InputEvent::KeyPressed:
                    {
                        ControllerButtonPressedEvent event(ctrlID, button);
                        EventCallback(event);
                    }
                    break;
                case InputEvent::KeyReleased:
                    {
                        ControllerButtonReleasedEvent event(ctrlID, button);
                        EventCallback(event);
                    }
                    break;
                case InputEvent::KeyDouble:
                    {
                        ControllerButtonDoubleClickEvent event(ctrlID, button);
                        EventCallback(event);
                    }
                    break;
                case InputEvent::KeyClicked:
                    {
                        ControllerButtonClickedEvent event(ctrlID, button);
                        EventCallback(event);
                    }
                    break;
            }
        }
        // TRiggers
        {
            // Left
            {
                ControllerButton button = ControllerButton::ButtonLeftTrigger;
                InputEvent action = ControllerGetState(controller, state, button);
                controller.LeftTrigger.State = action;
                switch (action)
                {
                    case InputEvent::KeyPressed:
                    {
                        ControllerButtonPressedEvent event(ctrlID, button);
                        EventCallback(event);
                        break;
                    }
                    case InputEvent::KeyReleased:
                    {
                        ControllerButtonReleasedEvent event(ctrlID, button);
                        EventCallback(event);
                        break;
                    }
                    case InputEvent::KeyDouble:
                    {
                        ControllerButtonDoubleClickEvent event(ctrlID, button);
                        EventCallback(event);
                        break;
                    }
                    case InputEvent::KeyClicked:
                    {
                        ControllerButtonClickedEvent event(ctrlID, button);
                        EventCallback(event);
                        break;

                    }
                }
            }
            // RIGHT
            {
                ControllerButton button = ControllerButton::ButtonRightTrigger;
                InputEvent action = ControllerGetState(controller, state, button);
                controller.RightTrigger.State = action;
                switch (action) {
                case InputEvent::KeyPressed:
                {
                    ControllerButtonPressedEvent event(ctrlID, button);
                    EventCallback(event);
                }
                break;
                case InputEvent::KeyReleased:
                {
                    ControllerButtonReleasedEvent event(ctrlID, button);
                    EventCallback(event);
                }
                break;
                case InputEvent::KeyDouble:
                {
                    ControllerButtonDoubleClickEvent event(ctrlID, button);
                    EventCallback(event);
                }
                break;
                case InputEvent::KeyClicked:
                {
                    ControllerButtonClickedEvent event(ctrlID, button);
                    EventCallback(event);
                }
                break;
                }
            }
        }
        // Playstation ONLY
        {
            {
                // 13 IS TOUCHPAD CLICK

            }
        }
    }

    void WindowsWindow::ContollerAxisCallback(Controller& controller) {
        GLFWgamepadstate state;
        if (glfwGetGamepadState(controller.ID, &state) == false)
            return;
        int ctrlID = controller.ID;
        // LEFT ANALOUGE STICK
        {
            float x = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
            float y = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
            if ((x > 0.2 || x < -0.2) || (y > 0.2 || y < -0.2))
            { // in the futre 0.2 would be deadzone
                ControllerLeftJoystickAxisEvent event(ctrlID, x, y, x - controller.LeftJoystick.Axis.X, controller.LeftJoystick.Axis.Y - y);
                EventCallback(event);
            }
            controller.LeftJoystick.Axis.X = x;
            controller.LeftJoystick.Axis.Y = y;
        }
        // RIGHT ANALOUGE STICK
        {
            float x = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
            float y = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
            if ((x > 0.2 || x < -0.2) || (y > 0.2 || y < -0.2))
            { // in the futre 0.2 would be deadzone
                ControllerLeftJoystickAxisEvent event(ctrlID, x, y, x - controller.RightJoystick.Axis.X, controller.RightJoystick.Axis.Y - y);
                EventCallback(event);
            }
            controller.RightJoystick.Axis.X = x;
            controller.RightJoystick.Axis.Y = y;
        }
        //LEFT TRIGGER
        {
            float trigger = state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
            if (trigger > -1)
            { // in the futre any value bigger than -1  would be deadzone
                ControllerTriggerAxisEvent event(ctrlID, trigger, trigger - controller.LeftTrigger.Axis, ControllerAxis::LeftTrigger);
                EventCallback(event);

            }
            controller.LeftTrigger.Axis = trigger;
        }
        //Right TRIGGER
        {
            float trigger = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
            if (trigger > -1)
            { // in the futre any value bigger than -1  would be deadzone
                ControllerTriggerAxisEvent event(ctrlID, trigger, trigger - controller.LeftTrigger.Axis, ControllerAxis::RightTrigger);
                EventCallback(event);

            }
            controller.LeftTrigger.Axis = trigger;
        }
    }

    void WindowsWindow::FrameBufferResizedCallback(int width, int height) {
        m_FrameBufferResized = true;
    }


    void WindowsWindow::Mouse_Moved_Callback(double xpos, double ypos) {
        MouseMoveEvent mouseEvent(xpos, ypos, xpos - m_MousePreviousLocationX, m_MousePreviousLocationY - ypos);
        EventCallback(mouseEvent);
        m_MousePreviousLocationX = xpos;
        m_MousePreviousLocationY = ypos;
        m_MouseMoved = true;
    }

    void WindowsWindow::Mouse_Hover_Window(int entered) {
    }

    void WindowsWindow::Mouse_ScrollWhell_Callback(double xoffset, double yoffset) {
        if (m_InputEventEnabled == false)
            return;
        MouseScrollEvent mouseEvent(xoffset, yoffset);
        EventCallback(mouseEvent);
        MouseScrollX.emplace_back(xoffset);
        MouseScrollY.emplace_back(yoffset);
    }
    static void SetJoystickCallback(WindowsWindow* window, int cID, int event)
    {
        glfwSetJoystickUserPointer(cID, window);

    }
    int WindowsWindow::Init() {

        if (s_GLFWWindowCount == 0)
        {
            if (!glfwInit())
            {
                int success = glfwInit();
                PF_CORE_ASSERT(success, "Could not initialize GLFW!");
                glfwSetErrorCallback(GLFWErrorCallback);
            }
        }
        if (Renderer::GetAPI() == Renderer::API::Vulkan)
        {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // so we do not set an api as open gl
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        }
        glfwWindowHint(GLFW_DECORATED, m_WindowConfiguration.Decorated);
        if (m_WindowConfiguration.startWindowedFullScreen)
        {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
            m_Window = glfwCreateWindow(mode->width, mode->height, m_WindowConfiguration.Title.c_str(), glfwGetPrimaryMonitor(), NULL);
            int width, height;
            glfwGetWindowSize((GLFWwindow*)m_Window, &width, &height);
            m_WindowConfiguration.Width = width;
            m_WindowConfiguration.Height = height;
        }
        else
            m_Window = glfwCreateWindow(m_WindowConfiguration.Width, m_WindowConfiguration.Height, m_WindowConfiguration.Title.c_str(), nullptr, NULL);
        s_GLFWWindowCount++;
        if (m_Window == nullptr)
        {
            PF_CORE_ASSERT(false, "Window  Nullptr");
            glfwTerminate();
            return -1;
        }
        glfwSetWindowUserPointer((GLFWwindow*)m_Window, this);

        if (m_WindowConfiguration.startFullScreen && m_WindowConfiguration.startWindowedFullScreen == false)
        {
            glfwMaximizeWindow((GLFWwindow*)m_Window);
            int width, height;
            glfwGetWindowSize((GLFWwindow*)m_Window, &width, &height);
            m_WindowConfiguration.Width = width;
            m_WindowConfiguration.Height = height;
        }

        if (m_WindowConfiguration.startFullScreen == false && m_WindowConfiguration.startWindowedFullScreen == false)
        {
            CenterWindow();
        }
        glfwSetKeyCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int key, int scancode, int action, int mods)mutable {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.key_callback(key, scancode, action, mods);
        });

        glfwSetMouseButtonCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int button, int action, int mods) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.mouse_button_callback(button, action, mods);
        });
        glfwSetCursorPosCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, double xpos, double ypos) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Mouse_Moved_Callback(xpos, ypos);
        });
        glfwSetCursorEnterCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int entered) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Mouse_Hover_Window(entered);
        });
        glfwSetScrollCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, double xoffset, double yoffset) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Mouse_ScrollWhell_Callback(xoffset, yoffset);
        });

        glfwSetWindowSizeCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int width, int height) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Window_Resize_Callback(width, height);
        });

        glfwSetWindowPosCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int xpos, int ypos) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Window_Position_Callback(xpos, ypos);
        });
        glfwSetWindowCloseCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Window_Close_Callback();
        });

        glfwSetWindowFocusCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int focused) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Window_Input_Focus_callback(focused);
        });
        glfwSetFramebufferSizeCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int width, int height) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.FrameBufferResizedCallback(width, height);
        });


        glfwSetJoystickCallback([](int cID, int event)
        {
            //SetJoystickCallback

            WindowsWindow* proofWindow = (WindowsWindow*)glfwGetJoystickUserPointer(cID);
           // WindowsWindow& data = *static_cast<WindowsWindow*>(glfwGetJoystickUserPointer(cID));
            proofWindow->ControllerCallbackConnect(cID, event);
        });

        PF_ENGINE_INFO("Window created widht {} height {}", m_WindowConfiguration.Width, m_WindowConfiguration.Height);
        return 0;
    }

    void WindowsWindow::CenterWindow() {
        int maxWidth = GetSystemMetrics(SM_CXSCREEN);
        int maxHeight = GetSystemMetrics(SM_CYSCREEN);
        glfwSetWindowPos((GLFWwindow*)m_Window, (maxWidth / 2) - (m_WindowConfiguration.Width / 2), (maxHeight / 2) - (m_WindowConfiguration.Height / 2));
    }

    void WindowsWindow::SetVsync(bool vsync) {
        m_WindowConfiguration.Vsync = vsync;
        glfwSwapInterval((int)m_WindowConfiguration.Vsync);
    }

    Vector2 WindowsWindow::GetMousePosition() {
        VectorTemplate2<double> pos;
        glfwGetCursorPos((GLFWwindow*)m_Window, &pos.X, &pos.Y);
        return Vector2{ (float)pos.X, (float)pos.Y };
    }

    int WindowsWindow::End() {
        glfwDestroyWindow((GLFWwindow*)m_Window);
        s_GLFWWindowCount--;
        if (s_GLFWWindowCount == 0)
        {
            glfwTerminate();
        }
        m_Window = nullptr;
        return 0;
    }
}
