
#include "Proofprch.h"
#include "Application.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include "Proof/Events/Event.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Resources/Asset/AssetManager.h"

namespace Proof {
    WindowsWindow* Application::MainWindow = nullptr;
    Count<ScreenFrameBuffer> Application::ScreenFrameBuffer = nullptr;
    uint32_t Application::ViewPortWidth;
    uint32_t Application::ViewPortHeight;
    float Application::FPS = 60.0f;
    float Application::FrameMS = 2.0f;
    Application::Application(){
        srand(time(NULL));
        Proof::Log::Init();
        MainWindow = new WindowsWindow(); // this is the size of our current second monitor
        MainWindow->createWindow();

        m_GraphicsContext =GraphicsContext::Create(static_cast<Window*>(MainWindow));
        m_GraphicsContext->Init();
        Renderer::Init();
        ImGuiMainLayer = new ImGuiLayer();
        MainLayerStack.PushLayer(ImGuiMainLayer);

        ScreenFrameBuffer = ScreenFrameBuffer::Create(1300,600);
        ScreenFrameBuffer->UnBind();
        //AssetManager::InitilizeAssets("content");
        
        MainWindow->SetEventCallback(PF_BIND_FN(Application::OnEvent));
    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowMinimizeEvent>(PF_BIND_FN(Application::OnWindowMinimizeEvent));
        dispatcher.Dispatch<MouseScrollEvent>(PF_BIND_FN(Application::OnMouseScrollEVent));
        
        if(e.GetCategoryFlags()==EventCategoryInput){
            PF_ENGINE_INFO("that is a gg");
        }
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
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
     //   glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ZERO);
       // CurrentWindow::SetSwapInterval(true);
        CurrentWindow::SetvSync(false);
        while (glfwWindowShouldClose(CurrentWindow::GetWindow()) == false && Input::IsKeyClicked(KeyBoardKey::Escape)==false) {
            float FrameStart = glfwGetTime();
            float time = (float)glfwGetTime();
            CurrentTime = glfwGetTime();
            FrameCount++;
            const FrameTime DeltaTime = time - LastFrameTime;
            RendererCommand::Enable(ProofRenderTest::DepthTest);
            if (WindowMinimized == false) 
            {
                ScreenFrameBuffer->Bind();
                RendererCommand::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);
                RendererCommand::SetClearColor(0.1f,0.1f,0.1f,1.0f);

                for (Layer* layer : MainLayerStack.V_LayerStack)
                    layer->OnUpdate(DeltaTime);
                Renderer::Draw();
                ScreenFrameBuffer->UnBind();
            }
            ImGuiMainLayer->Begin();
            for (Layer* layer : MainLayerStack.V_LayerStack)
                layer->OnImGuiDraw();
            ImGuiMainLayer->End();

            Renderer::Reset();
            MainWindow->WindowUpdate();
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