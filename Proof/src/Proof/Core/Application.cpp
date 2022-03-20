
#include "Proofprch.h"
#include "Application.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include "Proof/Events/Event.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Resources/Asset/AssetManager.h"
#include "Proof/Resources/Math/Random.h"
#include <GLFW/glfw3.h>
#include "Proof/Input/InputManager.h"
namespace Proof {
    Special <WindowsWindow> Application::MainWindow = nullptr;
    float Application::FPS = 60.0f;
    float Application::FrameMS = 2.0f;
    Application::Application(){
        srand(time(NULL));
        Proof::Log::Init();
        MainWindow = CreateSpecial<WindowsWindow>(); 

        Renderer::Init(static_cast<Window*>(MainWindow.get()));

        MainWindow->SetEventCallback(PF_BIND_FN(Application::OnEvent));
        if (Renderer::GetAPI() != RendererAPI::API::Vulkan) {
            AssetManager::NewInitilizeAssets("config/AssetManager.ProofAssetManager");
            ImGuiMainLayer = new ImGuiLayer();
            MainLayerStack.PushLayer(ImGuiMainLayer);
        }
        //AssetManager::InitilizeAssets("content");
        /*
        InputManager::AddMotion("MoveX");
        InputManager::AddMotion("MoveY");
        InputManager::MotionAddKey("MoveX", MotionInputType(InputDevice::KeyBoard, (int)KeyBoardKey::A, -1));
        InputManager::MotionAddKey("MoveX", MotionInputType(InputDevice::KeyBoard, (int)KeyBoardKey::D));
        InputManager::MotionAddKey("MoveX", MotionInputType(InputDevice::ControllerAxis, (int)ControllerAxis::LeftX));

        InputManager::MotionAddKey("MoveY", MotionInputType(InputDevice::KeyBoard, (int)KeyBoardKey::W));
        InputManager::MotionAddKey("MoveY", MotionInputType(InputDevice::KeyBoard, (int)KeyBoardKey::S, -1));
        InputManager::MotionAddKey("MoveY", MotionInputType(InputDevice::ControllerAxis, (int)ControllerAxis::LeftY));

        InputManager::AddAction("CursorEnabled");
        InputManager::AddAction("CursorDisabled");
        InputManager::ActionAddKey("CursorEnabled", InputType(InputDevice::MouseButton, (int)MouseButton::ButtonRight));
        InputManager::ActionAddKey("CursorDisabled", InputType(InputDevice::MouseButton, (int)MouseButton::ButtonRight));


        InputManager::AddMotion("RotateX");
        InputManager::AddMotion("RotateY");
        InputManager::MotionAddKey("RotateX", MotionInputType(InputDevice::MouseMovement, (int)MouseMovementInput::X, 1));
        InputManager::MotionAddKey("RotateY", MotionInputType(InputDevice::MouseMovement, (int)MouseMovementInput::Y, 1));

        InputManager::MotionAddKey("RotateY", MotionInputType(InputDevice::ControllerAxis, (int)ControllerAxis::RightY, 1));
        InputManager::MotionAddKey("RotateX", MotionInputType(InputDevice::ControllerAxis, (int)ControllerAxis::RightX, 1));
        */

        PF_ENGINE_TRACE("Engine Load Done");
    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowMinimizeEvent>(PF_BIND_FN(Application::OnWindowMinimizeEvent));
        dispatcher.Dispatch<MouseScrollEvent>(PF_BIND_FN(Application::OnMouseScrollEVent));
        dispatcher.Dispatch<ControllerButtonDoubleClickEvent>([](ControllerButtonDoubleClickEvent& e) {
            PF_INFO(e.ToString().c_str());
        });
        dispatcher.Dispatch<ControllerConnectEvent>([](auto& e) {
            PF_INFO(e.ToString().c_str());
        });
        dispatcher.Dispatch<ControllerDisconnectEvent>([](auto& e) {
            PF_INFO(e.ToString().c_str());
        });
        dispatcher.Dispatch<ControllerButtonPressedEvent>([](auto& e) {
            //PF_INFO(e.ToString().c_str());
        });
        dispatcher.Dispatch<ControllerButtonClickedEvent>([](auto& e) {
            PF_INFO(e.ToString().c_str());
        });
        dispatcher.Dispatch<ControllerButtonReleasedEvent>([](auto& e) {
            PF_INFO(e.ToString().c_str());
        });
         for (Layer* layer : MainLayerStack.V_LayerStack)
            layer->OnEvent(e);
    }

    void Application::OnWindowMinimizeEvent(WindowMinimizeEvent& e) {
        if(e.IsWIndowMinimized())
            WindowMinimized = true;
        else
            WindowMinimized = false;
    }

    void Application::OnMouseScrollEVent(MouseScrollEvent& e) {
    }

    void Application::OnKeyClicked(KeyClickedEvent& e) {
    }

    Application::~Application() {

    }

    void Application::Run() {

        uint64_t FrameCount = 0;
        float PreviousTime = glfwGetTime();
        float CurrentTime;
        if (Renderer::GetAPI() != RendererAPI::API::Vulkan)
            glEnable(GL_BLEND);
     //   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
     //   glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ZERO);

        CurrentWindow::SetvSync(false);

        while (glfwWindowShouldClose((GLFWwindow*)CurrentWindow::GetWindowAPI()) == false && Input::IsKeyClicked(KeyBoardKey::Escape)==false) {
            float FrameStart = glfwGetTime();
            float time = (float)glfwGetTime();
            CurrentTime = glfwGetTime();
            FrameCount++;
            const FrameTime DeltaTime = time - LastFrameTime;
            FrameTime::WorldDeltaTime = DeltaTime;
            if (Renderer::GetAPI() != RendererAPI::API::Vulkan)
                RendererCommand::Enable(ProofRenderTest::DepthTest);

            if (WindowMinimized == false) 
            {
                for (Layer* layer : MainLayerStack.V_LayerStack)
                    layer->OnUpdate(DeltaTime);
            }
            if (Renderer::GetAPI() != RendererAPI::API::Vulkan) {

                ImGuiMainLayer->Begin();
                for (Layer* layer : MainLayerStack.V_LayerStack)
                    layer->OnImGuiDraw(DeltaTime);
                ImGuiMainLayer->End();
            }

            MainWindow->WindowUpdate();
            if (CurrentTime - PreviousTime >= 1.0) {
                PreviousTime = CurrentTime;
                FrameCount = 0;
            }
            FPS = (1.0 / (CurrentTime - PreviousTime)) * FrameCount;
            FrameMS = ((CurrentTime - PreviousTime) / FrameCount) * 1000;

            LastFrameTime = time;
        };
        IsRunning = false;
        if (Renderer::GetAPI() != RendererAPI::API::Vulkan)
            AssetManager::SaveAllAsset();
    }

    void Application::PushLayer(Layer* Layer) {
        MainLayerStack.PushLayer(Layer);
    }

    void Application::PushOverlay(Layer* Layer) {
        MainLayerStack.PushOverlay(Layer);
    }
}