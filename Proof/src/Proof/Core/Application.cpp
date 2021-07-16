
#include "Proofprch.h"
#include "Application.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include "Proof/Events/Event.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Core/EditorMousePicking.h"
namespace Proof {
    WindowsWindow* Application::MainWindow = nullptr;
    Count<FrameBuffer> Application::ScreenFrameBuffer = nullptr;
    uint32_t Application::ViewPortWidth;
    uint32_t Application::ViewPortHeight;
    float Application::FPS = 60.0f;
    float Application::FrameMS = 2.0f;
    Application::Application(){
        MainWindow = new WindowsWindow(1300,600);
        

        MainWindow->createWindow();
        m_GraphicsContext =GraphicsContext::Create(CurrentWindow::GetWindow());
        m_GraphicsContext->Init();
        Renderer::Init();
        ImGuiMainLayer = new ImGuiLayer();
        MainLayerStack.PushLayer(ImGuiMainLayer);

        ScreenFrameBuffer = FrameBuffer::Create(1300,600);
        ScreenFrameBuffer->UnBind();
       // m_MousePickingEditor= {1300,600};
    }

    Application::~Application() {

    }

    void Application::Run() {
        KeyClickedEvent _KeyClickedEvent;
        Window_ViewPortResize _WIndow_ViewPortResizeEvent;
        uint64_t FrameCount = 0;
        float PreviousTime = glfwGetTime();
        float CurrentTime;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ZERO);
        CurrentWindow::SetSwapInterval(true);
        while (glfwWindowShouldClose(CurrentWindow::GetWindow()) == false && _KeyClickedEvent.GetKeyClicked() != KeyBoardKey::Escape) {
            float FrameStart = glfwGetTime();
            float time = (float)glfwGetTime();
            CurrentTime = glfwGetTime();
            FrameCount++;
            const FrameTime DeltaTime = time - LastFrameTime;
            
            RendererCommand::EnableDepth(false);
            /*
            m_MousePickingEditor.EnableWriting();
            RendererCommand::SetClearColor(0.1f,0.1f,0.1f,1.0f);
            RendererCommand::Clear();
            if(Input::IsMouseButtonPressed(MouseButton::ButtonLeft)){
                PF_ENGINE_INFO("%i",m_MousePickingEditor.ReadPixel(Input::GetMousePosX(),Input::GetMouswPosY()));
            }
            m_MousePickingEditor.DisbleWriting();
            RendererCommand::EnableDepth(true);
            */
            //Pick
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
                
                /*
                if(Input::IsMouseButtonPressed(MouseButton::ButtonLeft)){
                    glBindFramebuffer(GL_READ_FRAMEBUFFER,ScreenFrameBuffer->GetFrameBufferID());
                    glReadBuffer(GL_COLOR_ATTACHMENT0); // reading from zero colour attachment
                    float Pixels[3];
                    glReadPixels(Input::GetMousePosX(),Input::GetMousePosX(),1,1,GL_RGB,GL_FLOAT,Pixels);
                    PF_ENGINE_INFO("%i",(int)Pixels[2]);
                }
                */
                for (Layer* layer : MainLayerStack.V_LayerStack)
                    layer->OnUpdate(DeltaTime);
                Renderer::Draw();

                ScreenFrameBuffer->UnBind();
                MainWindow->WindowUpdate(DeltaTime);
            }


            Renderer::Reset();
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