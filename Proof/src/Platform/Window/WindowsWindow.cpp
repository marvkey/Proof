#include "Proofprch.h"
#include "WindowsWindow.h"
#include "Proof/Input/KeyCodes.h"
#include "Proof/Events/MouseEvent.h"
#include  "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Events/ControllerEvent.h"
#include "Proof/Core/CurrentWindow.h"

#include "Proof/Core/FrameTime.h"

#include "Proof/Renderer/GraphicsContext.h"
#include "Proof/Core/Core.h"
#include "ImGui/imgui.h"
#include <GLFW/glfw3.h>

namespace Proof {



    WindowsWindow::WindowsWindow(unsigned int Width, unsigned int Height) {
        this->Width = Width;
        this->Height = Height;
        // setting all values to false
        for (int i = 0; i < KeyPressed.size(); i++) {
            KeyPressed[i] = false;
        }
        for (int i = 0; i < MouseButtonPressed.size(); i++) {
            MouseButtonPressed[i] = false;
        }
        this->createWindow();
        m_MousePreviousLocationX = Input::GetMousePosX();
        m_MousePreviousLocationY = Input::GetMousePosY();
    }
    void WindowsWindow::WindowUpdate() {
        glfwSwapInterval(Vsync);
        
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
            for (int i = 0; i < m_KeyPressedEventCheck.size(); i++) {
                KeyBoardKey key = m_KeyPressedEventCheck[i];
                if (glfwGetKey((GLFWwindow*)CurrentWindow::GetWindowAPI(), (int)key)) {
                    KeyPressedEvent pressedEvent(key);
                    EventCallback(pressedEvent);
                }
                else {
                    KeyPressed[(int)key] = false;
                    m_KeyPressedEventCheck.erase(m_KeyPressedEventCheck.begin() + i);
                }
            }

            for (int i = 0; i < m_MouseButtonPressedEventCheck.size(); i++) {
                MouseButton key = m_MouseButtonPressedEventCheck[i];
                if (glfwGetMouseButton((GLFWwindow*)CurrentWindow::GetWindowAPI(), (int)key)) {
                    MouseButtonPressedEvent pressedEvent(key);
                    EventCallback(pressedEvent);
                }
                else {
                    MouseButtonPressed[(int)key] = false;
                    m_MouseButtonPressedEventCheck.erase(m_MouseButtonPressedEventCheck.begin() + i);
                }
            }
        }
        glfwSwapBuffers((GLFWwindow*)m_Window);
        glfwPollEvents();
    }
    void WindowsWindow::key_callback(int key, int scancode, int action, int mods) {
        /* This is for when a key is Clicked 2 */
        if (action == GLFW_RELEASE) {
            static auto before = std::chrono::system_clock::now();
            auto now = std::chrono::system_clock::now();
            double diff_ms = std::chrono::duration <double, std::milli>(now - before).count();
            before = now;
            if (diff_ms > 10 && diff_ms < 200) {
                action = (int)InputEvent::KeyDouble;
            }
        }     
        
        switch (action) {
        case GLFW_PRESS:
            KeyboardClicked.emplace_back((KeyBoardKey)key);
            {
                // have to use this because GLFW someties will not send that a key hasb een released

                KeyClickedEvent keyevent((KeyBoardKey)key);
                EventCallback(keyevent);
              
                if (key < KeyPressed.size()) {
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
                if (key < KeyPressed.size()) {
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
        if (action == GLFW_RELEASE) {
            static auto before = std::chrono::system_clock::now();
            auto now = std::chrono::system_clock::now();
            double diff_ms = std::chrono::duration <double, std::milli>(now - before).count();
            before = now;
            if (diff_ms > 10 && diff_ms < 200) {
                action = (int)InputEvent::KeyDouble;
            }
        }
        switch (action) {
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
      
    void WindowsWindow::Window_Resize_Callback(int width, int height){
        WindowResizeEvent windowResizeEvent(width,height);
        EventCallback(windowResizeEvent);
        if(width==0 || height==0){
            WindowMinimizeEvent Minimizedevent(true);
            EventCallback(Minimizedevent);

        }else if (Width==0 || Height==0){
            WindowMinimizeEvent Minimizedevent(false);
            EventCallback(Minimizedevent);
        }
        Width = width;
        Height = height;
    }

    void WindowsWindow::Window_Position_Callback(int xpos, int ypos){
        WindowMoveEvent windowEvent(xpos,ypos);
        EventCallback(windowEvent);
    }

    void WindowsWindow::Window_Close_Callback(){
        WindowCloseEvent windowEvent(true);
        EventCallback(windowEvent);
    }

    void WindowsWindow::Controller_Callback(int jid, int event){
        // TODO NEED TO FIGURE IF WE 
        // HAVE 4 CONTROLLERS
        // WE REMOVE CONTROLLER 2 
        // DOES CONTROLLER 3 THEN GET CHANGED TO CONTROLLER 2 
        // AND CONTROLLLER 4 GETS CHANGED TO CONTROLLER 3
        if(event == GLFW_CONNECTED) // because when we callback when removed it is removed so checking if gamepad to glfw the controller is alreayd removed
            if (glfwJoystickIsGamepad(jid) == false)return; // if we are using a controller
        if (event == GLFW_CONNECTED) {
            CurrentWindow::GetWindowClass().m_Controllers.emplace_back(Controller());
            Controller& newController = CurrentWindow::GetWindowClass().m_Controllers.back();
            newController.ID = jid;
            newController.Name = glfwGetGamepadName(jid);
            for (int i = 0; i < newController.Buttons.size(); i++) {
                newController.Buttons[i] = 0;
            }
            ControllerConnectEvent ctEvent(jid);
            CurrentWindow::GetWindowClass().EventCallback(ctEvent);
        }
        else if (event == GLFW_DISCONNECTED) {
            for (int i = 0; i < CurrentWindow::GetWindowClass().m_Controllers.size(); i++) {
                if (CurrentWindow::GetWindowClass().m_Controllers[i].ID == jid) {
                    CurrentWindow::GetWindowClass().m_Controllers.erase(CurrentWindow::GetWindowClass().m_Controllers.begin() + i);
                    break;
                }
            }
            ControllerDisconnectEvent ctEvent(jid);
            CurrentWindow::GetWindowClass().EventCallback(ctEvent);
        }
    }

    void WindowsWindow::Window_Input_Focus_callback(int focused){
        WindowFocusEvent windowEvent(true);
        EventCallback(windowEvent);
    }

    void WindowsWindow::ControllerEventCallback(){
        for (Controller& controller : m_Controllers) {
            ContollerButtonCallback(controller);
            ContollerAxisCallback(controller);
        }
    }
    static InputEvent ControllerGetState(Controller& controller, GLFWgamepadstate& state, ControllerButton button) {
        static std::unordered_map<int, int> s_ControllerLastKeyClicked; // the last key for controller register

        if ((button == ControllerButton::ButtonLeftTrigger) || (button == ControllerButton::ButtonRightTrigger))
            goto trigerCheck;
        {
            int buttonIndex = (int)button;
            auto it = s_ControllerLastKeyClicked.find(controller.ID);
            if (it == s_ControllerLastKeyClicked.end()) {
                s_ControllerLastKeyClicked.insert({ controller.ID,-1 });
            }

            if (state.buttons[buttonIndex] == GLFW_PRESS) {
                if (controller.Buttons[buttonIndex] == GLFW_PRESS || controller.Buttons[buttonIndex] == (int)InputEvent::KeyClicked) // THE KEY WAS ALSO PRESSED last FRAME
                    return InputEvent::KeyPressed;
                if (controller.Buttons[buttonIndex] == (int)InputEvent::None) { // THE KEY WAS NOT PRESSED LAST FRAME
                    auto it = s_ControllerLastKeyClicked.find(controller.ID);
                    if (it != s_ControllerLastKeyClicked.end()) {
                        it->second = buttonIndex;
                    }
                    else {
                        s_ControllerLastKeyClicked.insert({ controller.ID,buttonIndex });
                    }
                    return InputEvent::KeyClicked;
                }
            }
            if (state.buttons[buttonIndex] == GLFW_RELEASE) {
                if (buttonIndex == s_ControllerLastKeyClicked.at(controller.ID)) { // double click checking
                    if (controller.Buttons[buttonIndex] == (int)InputEvent::KeyClicked || controller.Buttons[buttonIndex] == (int)InputEvent::KeyPressed) {
                        static auto before = std::chrono::system_clock::now();
                        auto now = std::chrono::system_clock::now();
                        double diff_ms = std::chrono::duration <double, std::milli>(now - before).count();
                        before = now;

                        if (diff_ms > 10 && diff_ms < 400) {
                            s_ControllerLastKeyClicked.at(controller.ID) = -1;
                            return InputEvent::KeyDouble;
                        }
                    }
                }
                // if not double click that means key is released
                if (controller.Buttons[buttonIndex] == (int)InputEvent::KeyClicked || controller.Buttons[buttonIndex] == (int)InputEvent::KeyPressed) { // THE button WAS ALSO PRESSED last FRAME
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
                if (button == ControllerButton::ButtonLeftTrigger) {
                    if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > 0) { // if the key is pressed
                        if (controller.m_ButtonLeftTriggerr == GLFW_PRESS || controller.m_ButtonLeftTriggerr == (int)InputEvent::KeyClicked)
                            return InputEvent::KeyPressed;
                        if (controller.m_ButtonLeftTriggerr == (int)InputEvent::None) { // THE KEY WAS NOT PRESSED LAST FRAME
                            auto it = s_ControllerLastKeyClicked.find(controller.ID);
                            if (it != s_ControllerLastKeyClicked.end()) {
                                it->second = (int)ControllerButton::ButtonLeftTrigger;
                            }
                            else {
                                s_ControllerLastKeyClicked.insert({ controller.ID,(int)ControllerButton::ButtonLeftTrigger });
                            }
                            return InputEvent::KeyClicked;
                        }

                    }
                    if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] < 0) { // key is released
                        if ((int)ControllerButton::ButtonLeftTrigger == s_ControllerLastKeyClicked.at(controller.ID)) { // double click checking
                            if (controller.m_ButtonLeftTriggerr == (int)InputEvent::KeyClicked || controller.m_ButtonLeftTriggerr == (int)InputEvent::KeyPressed) { // double click checking
                                static auto before = std::chrono::system_clock::now();
                                auto now = std::chrono::system_clock::now();
                                double diff_ms = std::chrono::duration <double, std::milli>(now - before).count();
                                before = now;
                                
                                if (diff_ms > 10 && diff_ms < 400) {
                                    s_ControllerLastKeyClicked.at(controller.ID) = -1;
                                    return InputEvent::KeyDouble;
                                }
                            }
                        }
                        if (controller.m_ButtonLeftTriggerr == (int)InputEvent::KeyClicked || controller.m_ButtonLeftTriggerr == (int)InputEvent::KeyPressed) { // THE button WAS ALSO PRESSED last FRAME
                            return InputEvent::KeyReleased;
                        }
                    }
                    return InputEvent::None; // left trigger non
                }
            }
            // RIGHT TRIGGER
            {
                if (button == ControllerButton::ButtonRightTrigger) {
                    if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > 0) { // if the key is pressed
                        if (controller.m_ButtonRightTrigger== GLFW_PRESS || controller.m_ButtonRightTrigger == (int)InputEvent::KeyClicked)
                            return InputEvent::KeyPressed;
                        if (controller.m_ButtonRightTrigger == (int)InputEvent::None) { // THE KEY WAS NOT PRESSED LAST FRAME
                            auto it = s_ControllerLastKeyClicked.find(controller.ID);
                            if (it != s_ControllerLastKeyClicked.end()) {
                                it->second = (int)ControllerButton::ButtonRightTrigger;
                            }
                            else {
                                s_ControllerLastKeyClicked.insert({ controller.ID,(int)ControllerButton::ButtonRightTrigger });
                            }
                            return InputEvent::KeyClicked;
                        }

                    }
                    if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] < 0) { // key is released
                        if ((int)ControllerButton::ButtonRightTrigger == s_ControllerLastKeyClicked.at(controller.ID)) { // double click checking
                            if (controller.m_ButtonRightTrigger == (int)InputEvent::KeyClicked || controller.m_ButtonRightTrigger == (int)InputEvent::KeyPressed) { // double click checking
                                static auto before = std::chrono::system_clock::now();
                                auto now = std::chrono::system_clock::now();
                                double diff_ms = std::chrono::duration <double, std::milli>(now - before).count();
                                before = now;

                                if (diff_ms > 10 && diff_ms < 400) {
                                    s_ControllerLastKeyClicked.at(controller.ID) =(int) ControllerButton::None;
                                    return InputEvent::KeyDouble;
                                }
                            }
                        }
                        if (controller.m_ButtonRightTrigger == (int)InputEvent::KeyClicked || controller.m_ButtonRightTrigger == (int)InputEvent::KeyPressed) { // THE button WAS ALSO PRESSED last FRAME
                            return InputEvent::KeyReleased;
                        }
                    }
                    return InputEvent::None; // right trigger non
                }
            }

        }
        return InputEvent::None; // incase
    }
    void WindowsWindow::ContollerButtonCallback(Controller& controller){
        static std::unordered_map<int, int> s_ControllerLastKeyClicked; // the last key for controller register
        GLFWgamepadstate state;

        if (glfwGetGamepadState(controller.ID, &state) == false)
            return;
        int ctrlID = controller.ID;
        for (int buttonCount = 0; buttonCount < controller.Buttons.size(); buttonCount++) { // +2 for the trigger kerys4
            InputEvent action = ControllerGetState(controller, state, (ControllerButton)buttonCount);
            ControllerButton button = (ControllerButton)buttonCount;
            controller.Buttons[buttonCount] = (int)action;

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
        // TRiggers
        {
            // Left
            {
                ControllerButton button = ControllerButton::ButtonLeftTrigger;
                InputEvent action = ControllerGetState(controller, state, button);
                controller.m_ButtonLeftTriggerr = (int)action;
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
            // RIGHT
            {
                ControllerButton button = ControllerButton::ButtonRightTrigger;
                InputEvent action = ControllerGetState(controller, state, button);
                controller.m_ButtonRightTrigger = (int)action;
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
        /*
        // RIGHT TRIGGER
        {
            if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] > 0) { // if the key is pressed
                if (controller.m_ButtonRightTrigger == GLFW_PRESS) {
                    ControllerButtonPressedEvent event(ctrlID,(ControllerButton)20);
                    EventCallback(event);
                }
                else if (controller.m_ButtonRightTrigger == GLFW_RELEASE) {
                    ControllerButtonClickedEvent event(ctrlID,(ControllerButton)20);
                    EventCallback(event);
                    auto it = s_ControllerLastKeyClicked.find(ctrlID);
                    if (it != s_ControllerLastKeyClicked.end()) {
                        it->second = 20;
                    }
                    else {
                        s_ControllerLastKeyClicked.insert({ ctrlID,20 });
                    }
                }
                controller.m_ButtonRightTrigger = GLFW_PRESS;
            }
            else if (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] < 0) { // key is not clicked
                if (controller.m_ButtonRightTrigger == GLFW_PRESS) { // released
                    if (controller.m_ButtonRightTrigger == s_ControllerLastKeyClicked.at(ctrlID)) { // double click checking
                        int action = GLFW_RELEASE;
                        static auto before = std::chrono::system_clock::now();
                        auto now = std::chrono::system_clock::now();
                        double diff_ms = std::chrono::duration <double, std::milli>(now - before).count();
                        before = now;
                        if (diff_ms > 10 && diff_ms < 400) {
                            action = (int)InputEvent::KeyDouble;
                        }

                        if (action == (int)InputEvent::KeyDouble) {
                            ControllerButtonDoubleClickEvent event(ctrlID, (ControllerButton)20);
                            EventCallback(event);
                            controller.m_ButtonRightTrigger = (int)InputEvent::KeyDouble;
                        }
                    }
                    else {
                        ControllerButtonReleasedEvent event(ctrlID, (ControllerButton)20);
                        EventCallback(event);
                        controller.m_ButtonRightTrigger = GLFW_RELEASE;
                    }
                }
            }
        }
        // LEFT TRIGGER
        {
            if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] > 0) { // if the key is pressed
                if (controller.m_ButtonLeftTriggerr == GLFW_PRESS) {
                    ControllerButtonPressedEvent event(ctrlID,(ControllerButton)21);
                    EventCallback(event);
                }
                else if (controller.m_ButtonLeftTriggerr == GLFW_RELEASE) { // clickde
                    ControllerButtonClickedEvent event(ctrlID,(ControllerButton)21);
                    EventCallback(event);
                    auto it = s_ControllerLastKeyClicked.find(ctrlID);
                    if (it != s_ControllerLastKeyClicked.end()) {
                        it->second = (int)ControllerButton::ButtonLeftTrigger;
                    }
                    else {
                        s_ControllerLastKeyClicked.insert({ ctrlID,(int)ControllerButton::ButtonLeftTrigger });
                    }
                }
                controller.m_ButtonLeftTriggerr = GLFW_PRESS;
            }
            else if (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] < 0) { // key is not clicked
                if (controller.m_ButtonLeftTriggerr == GLFW_PRESS) { // released
                    if (controller.m_ButtonLeftTriggerr == s_ControllerLastKeyClicked.at(ctrlID)) { // double click checking
                        int action = GLFW_RELEASE;
                        static auto before = std::chrono::system_clock::now();
                        auto now = std::chrono::system_clock::now();
                        double diff_ms = std::chrono::duration <double, std::milli>(now - before).count();
                        before = now;
                        if (diff_ms > 10 && diff_ms < 400) {
                            action = (int)InputEvent::KeyDouble;
                        }

                        if (action == (int)InputEvent::KeyDouble) {
                            ControllerButtonDoubleClickEvent event(ctrlID, (ControllerButton)controller.m_ButtonLeftTriggerr);
                            EventCallback(event);
                            controller.m_ButtonLeftTriggerr = (int)InputEvent::KeyDouble;
                        }
                    }
                    else {
                        ControllerButtonReleasedEvent event(ctrlID, (ControllerButton)21);
                        EventCallback(event);
                        controller.m_ButtonLeftTriggerr = GLFW_RELEASE;
                    }
                }
            }
        }
        // Playstation ONLY
        {
            { 
                // 13 IS TOUCHPAD CLICK
                
            }
        }
        */
    }

    void WindowsWindow::ContollerAxisCallback(Controller& controller) {
        GLFWgamepadstate state;
        if (glfwGetGamepadState(controller.ID, &state) == false)
            return;
        int ctrlID = controller.ID;
        // LEFT ANALOUGE STICK
        {
            float x  = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X]; 
            float y = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
            if ((x > 0.2 || x < -0.2) || (y > 0.2 || y < -0.2)) { // in the futre 0.2 would be deadzone
                ControllerLeftJoystickAxisEvent event(ctrlID,x, y, x - controller.LeftJoystickX, controller.LeftJoystickY - y);
                EventCallback(event);
            }
            controller.LeftJoystickX = x;
            controller.LeftJoystickY = y;
        }
        // RIGHT ANALOUGE STICK
        {
            float x = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
            float y = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
            if ((x > 0.2 || x < -0.2) || (y > 0.2 || y < -0.2)) { // in the futre 0.2 would be deadzone
                ControllerLeftJoystickAxisEvent event(ctrlID,x, y, x - controller.RightJoystickX, controller.RightJoystickY - y);
                EventCallback(event);
            }
            controller.RightJoystickX = x;
            controller.RightJoystickY= y;
        }
        //LEFT TRIGGER
        {
            float trigger = state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
            if (trigger > -1) { // in the futre any value bigger than -1  would be deadzone
                ControllerTriggerAxisEvent event(ctrlID,trigger, trigger - controller.LeftTriggerAxis, ControllerAxis::LeftTrigger);
                EventCallback(event);

            }
            controller.LeftTriggerAxis = trigger;
        }
        //Right TRIGGER
        {
            float trigger = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
            if (trigger > -1) { // in the futre any value bigger than -1  would be deadzone
                ControllerTriggerAxisEvent event(ctrlID,trigger, trigger - controller.LeftTriggerAxis, ControllerAxis::RightTrigger);
                EventCallback(event);

            }
            controller.RightTriggerAxis = trigger;
        }
    }

   
    void WindowsWindow::Mouse_Moved_Callback(double xpos, double ypos){
        MouseMoveEvent mouseEvent(xpos,ypos, xpos - m_MousePreviousLocationX, m_MousePreviousLocationY - ypos);
        EventCallback(mouseEvent);
        m_MousePreviousLocationX = xpos;
        m_MousePreviousLocationY = ypos;
    }

    void WindowsWindow::Mouse_Hover_Window(int entered){
    }

    void WindowsWindow::Mouse_ScrollWhell_Callback(double xoffset, double yoffset){
        if (m_InputEventEnabled == false)
            return;
        MouseScrollEvent mouseEvent(xoffset,yoffset);
        EventCallback(mouseEvent);
        MouseScrollX.emplace_back(xoffset);
        MouseScrollY.emplace_back(yoffset);
    }

    int WindowsWindow::createWindow() {
        if (!glfwInit()) {
            PF_CORE_ASSERT(false,"Could Not Initilize GLFW");
            return -1;
        }
       // glfwWindowHint(GLFW_MAXIMIZED,GLFW_TRUE); // when using this meathod teh window will have some glithy meathod

        m_Window = glfwCreateWindow(Width,Height, "Proof", nullptr,NULL);
        if (m_Window == nullptr) {
            PF_CORE_ASSERT(false,"Window Is Nullptr");
            glfwTerminate();
            return -1;
        }
        glfwSetWindowUserPointer((GLFWwindow*)m_Window, this);
        glfwMaximizeWindow((GLFWwindow*)m_Window);

        glfwGetWindowSize((GLFWwindow*)m_Window,&Width,&Height);
       
        glfwMakeContextCurrent((GLFWwindow*)m_Window);
        glfwSetKeyCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int key, int scancode, int action, int mods)mutable {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.key_callback(key, scancode, action, mods);
        });

        glfwSetMouseButtonCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int button, int action, int mods){
                WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
                proofWindow.mouse_button_callback(button,action, mods);
        });
        glfwSetCursorPosCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, double xpos, double ypos) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Mouse_Moved_Callback(xpos,ypos);
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
            proofWindow.Window_Resize_Callback(width,height);
        });

        glfwSetWindowPosCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int xpos, int ypos) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Window_Position_Callback(xpos,ypos);
        });
        glfwSetWindowCloseCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Window_Close_Callback();
        });
        
        glfwSetWindowFocusCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int focused) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Window_Input_Focus_callback(focused);
        });

        glfwSetJoystickCallback(WindowsWindow::Controller_Callback);
        PF_INFO("Window created widht %i height %i",Width,Height);
        return 0;
    }

    int WindowsWindow::WindowEnd() {
        glfwDestroyWindow((GLFWwindow*)m_Window);
        glfwTerminate();
        return 0;
    }
}
