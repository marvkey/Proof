
#include "Proofprch.h"
#include "Application.h"


#include "Platform/WindowsWindow.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof3D/Renderer/Camera/EditorCamera.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <ImGUIOpenGL/imgui.h>
#include <ImGUIOpenGL/imgui_impl_glfw.h>
#include <ImGUIOpenGL/imgui_impl_opengl3.h>

#include "Platform/OpenGL/Shader.h"
#include "Platform/OpenGL/Buffer.h"
#include "Platform/OpenGL/vertexArray.h"
#include "Platform/OpenGL/Texture.h"
namespace Proof {
    WindowsWindow* Application::MainWindow = nullptr;

    Application::Application() {
        MainWindow = new WindowsWindow(700, 700);
        MainWindow->createWindow();

        ImGuiMainLayer = new ImGuiLayer();
        MainLayerStack.PushLayer(ImGuiMainLayer);
    }

    Application::~Application() {
        delete MainWindow;
        PF_ENGINE_TRACE("window is deleted");
    }

    void Application::Run() {
        
        /*
        char full[_MAX_PATH];
        if (_fullpath(full,"../Proof/vertex.vs", _MAX_PATH) != NULL)
            printf("Full path is: %s\n", full);
        else
            printf("Invalid path\n");
            */
        glEnable(GL_DEPTH_TEST);
       Shader Shader("Proof/Core/vertex.vs", "Proof/Core/fragment.fs1");
     
       float vertices[] = {                       //Texture coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.3f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.3f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.3f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.3f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.3f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.3f,  0.0f, 0.0f,
                                          
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.3f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.3f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.3f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.3f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.3f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.3f,  0.0f, 0.0f,
                                          
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.3f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.3f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.3f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.3f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.3f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.3f,  1.0f, 0.0f,
                                          
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.3f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.3f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.3f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.3f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.3f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.3f,  1.0f, 0.0f,
                                          
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.3f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.3f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.3f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.3f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.3f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.3f,  0.0f, 1.0f,
                                          
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.3f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.3f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.3f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.3f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.3f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.3f,  0.0f, 1.0f
       };

       glm::vec3 cubePositions[] = {
       glm::vec3(0.0f,  0.0f,  0.0f),
       glm::vec3(2.0f,  5.0f, -15.0f),
       glm::vec3(-1.5f, -2.2f, -2.5f),
       glm::vec3(-3.8f, -2.0f, -12.3f),
       glm::vec3(2.4f, -0.4f, -3.5f),
       glm::vec3(-1.7f,  3.0f, -7.5f),
       glm::vec3(1.3f, -2.0f, -2.5f),
       glm::vec3(1.5f,  2.0f, -2.5f),
       glm::vec3(1.5f,  0.2f, -1.5f),
       glm::vec3(-1.3f,  1.0f, -1.5f)
       };

       
       VertexArray VertexArrayobj;
       VertexBuffer BufferObj;
       BufferObj.AddVertexBufferData(vertices,sizeof(vertices));

       VertexArrayobj.AddAtributePointer(0, 3, 8, 0);
       VertexArrayobj.AddAtributePointer(1, 3, 8, 3); 
       VertexArrayobj.AddAtributePointer(2, 2, 8, 6);
      
       Texture Contatiner("vendor/container.jpg");
       Texture HappyFace("vendor/awesomeface.png",true);

       Shader.UseShader();
       Shader.SetInt("texture1", 0);
       Shader.SetInt("texture2", 1);
       Shader.SetFloat("Transperance", 0.2);
       float PosX =0.0f, PosY =0.0f, PosZ= -2.0f;
       EditorCamera3D Camera;

        while ((glfwWindowShouldClose(CurrentWindow::GetWindow()) == false) && !(Input::IsKeyPressed(KeyBoardKey::Escape) ==true)) {
            float time = (float)glfwGetTime();
            const FrameTime DeltaTime = time - LastFrameTime;
            LastFrameTime = time;
            Contatiner.BindTexture(0);
            HappyFace.BindTexture(1);

            ImGuiMainLayer->Begin(); 
                for (Layer* layer : MainLayerStack.V_LayerStack)
                    layer->OnImGuiDraw();
            ImGuiMainLayer->End();

            for (Layer* layer : MainLayerStack.V_LayerStack)
                layer->OnUpdate(DeltaTime);


            Shader.UseShader();
            glm::mat4 View = glm::mat4(1.0f);
            glm::mat4 Projection = glm::mat4(1.0f);
            glm::mat4 Scale = glm::mat4(1.0f);

            if (Input::IsKeyClicked(KeyBoardKey::F));
            Projection = glm::perspective(glm::radians(45.f), (float)CurrentWindow::GetWindowWidth()/ (float)CurrentWindow::GetWindowHeight(), 0.1f, 100.0f);
            Scale = glm::scale(Scale, glm::vec3(1.0,1.0,1.0));
            Shader.SetMat4("Projection", Projection);
            Shader.SetMat4("Scale", Scale);
            Shader.SetMat4("View", Camera.GetCameraView());
            Camera.OnUpdate(DeltaTime);
            VertexArrayobj.BindVertexArray();

            for (glm::vec3& CubePositonarray : cubePositions) {
                glm::mat4 Model = glm::mat4(1.0f);
                float angle = 20.0f * CubePositonarray.x;
                Model = glm::rotate(Model, angle * glm::radians(55.0f), glm::vec3(1.0f, 0.3f, 0.5f));
                Model = glm::translate(Model, CubePositonarray);
                Shader.SetMat4("Model", Model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            MainWindow->WindowUpdate(DeltaTime); 
        };
        MainWindow->WindowEnd();
        delete MainWindow;
    }

    void Application::PushLayer(Layer* Layer){
        MainLayerStack.PushLayer(Layer);
    }

    void Application::PushOverlay(Layer* Layer){
        MainLayerStack.PushOverlay(Layer);
    }
}