
#include "Proofprch.h"
#include "Application.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include "Proof/Events/Event.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"

namespace Proof {
    WindowsWindow* Application::MainWindow = nullptr;
    Count<FrameBuffer> Application::ScreenFrameBuffer = nullptr;
    uint32_t Application::ViewPortWidth;
    uint32_t Application::ViewPortHeight;
    float Application::FPS = 60.0f;
    float Application::FrameMS = 2.0f;
    Application::Application() {
        MainWindow = new WindowsWindow(800,600);

        MainWindow->createWindow();
        m_GraphicsContext =GraphicsContext::Create(CurrentWindow::GetWindow());
        m_GraphicsContext->Init();
        Renderer::Init();
        ImGuiMainLayer = new ImGuiLayer();
        MainLayerStack.PushLayer(ImGuiMainLayer);

        ScreenFrameBuffer = FrameBuffer::Create(800,600);
        ScreenFrameBuffer->UnBind();
    }

    Application::~Application() {

    }

    void Application::Run() {
        KeyClickedEvent _KeyClickedEvent;
        Window_ViewPortResize _WIndow_ViewPortResizeEvent;
        uint64_t FrameCount = 0;
        float PreviousTime = glfwGetTime();
        float CurrentTime;
        RendererCommand::EnableDepth(true);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        //glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ZERO);
        CurrentWindow::SetSwapInterval(true);
        while (glfwWindowShouldClose(CurrentWindow::GetWindow()) == false && _KeyClickedEvent.GetKeyClicked() != KeyBoardKey::Escape) {
            float FrameStart = glfwGetTime();
            float time = (float)glfwGetTime();
            CurrentTime = glfwGetTime();
            FrameCount++;
            const FrameTime DeltaTime = time - LastFrameTime;
            if (CurrentWindow::GetWindowHeight() == 0 || CurrentWindow::GetWindowWidth() == 0)
                WindowMinimized = true;
            else
                WindowMinimized = false;
            ImGuiMainLayer->Begin();
            for (Layer* layer : MainLayerStack.V_LayerStack)
                layer->OnImGuiDraw();
            ImGuiMainLayer->End();

            if (WindowMinimized == false) {
                ScreenFrameBuffer->Bind();
                RendererCommand::Clear();
                RendererCommand::SetClearColor(0.1f,0.1f,0.1f,1.0f);

                for (Layer* layer : MainLayerStack.V_LayerStack)
                    layer->OnUpdate(DeltaTime);
                Renderer::Reset();

                ScreenFrameBuffer->UnBind();
                MainWindow->WindowUpdate(DeltaTime);
            }
            RendererCommand::SwapBuffer(CurrentWindow::GetWindow());
            RendererCommand::PollEvents();
            if (CurrentTime - PreviousTime >= 1.0) {
                PreviousTime = CurrentTime;
                FrameCount = 0;
            }
            FPS = (1.0 / (CurrentTime - PreviousTime)) * FrameCount;
            FrameMS = ((CurrentTime - PreviousTime) / FrameCount) * 1000;
            
            LastFrameTime = time;
        };
        m_GraphicsContext->CleanUp();
        MainWindow->WindowEnd();
        IsRunning = false;
        delete MainWindow;
    }

    void Application::PushLayer(Layer* Layer) {
        MainLayerStack.PushLayer(Layer);
    }

    void Application::PushOverlay(Layer* Layer) {
        MainLayerStack.PushOverlay(Layer);
    }
}