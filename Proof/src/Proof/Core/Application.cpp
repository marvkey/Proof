
#include "Proofprch.h"
#include "Application.h"
#include "Platform/WindowsWindow.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Renderer/Camera/EditorCamera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Proof/OpenGL/Shader.h"
#include "Proof/OpenGL/Buffer.h"
#include "Proof/OpenGL/VertexArray.h"
namespace Proof {
    WindowsWindow* Application::MainWindow = nullptr;

    Application::Application() {}

    Application::~Application() {}

    void Application::Run() {
        MainWindow = new WindowsWindow(700, 700);
        MainWindow->createWindow();
        
        /*
        char full[_MAX_PATH];
        if (_fullpath(full,"../Proof/vertex.vs", _MAX_PATH) != NULL)
            printf("Full path is: %s\n", full);
        else
            printf("Invalid path\n");
            */
        glEnable(GL_DEPTH_TEST);
       Shader Shader("Proof/Core/vertex.vs", "Proof/Core/fragment.fs1");

       float vertices[] = {
        0.5f,  0.5f, 0.0f,   0.5,0.0,0.0,
        0.5f, -0.5f, 0.0f,   0.0,.2,0.0,
       -0.5f, -0.5f, 0.0f,   0.0,0.0,.3,
       -0.5f,  0.5f, 0.0f,   0.0,0.0,.3,
       };
      

       unsigned int indices[] = {
           0, 1, 3, // first triangle
           1, 2, 3  // second triangle
       };

       VertexArray VertexArrayobj(1);
       VertexBuffer BufferObj(1);
       IndexBuffer BufferobjIndences(1);

       BufferObj.AddVertexBufferData(vertices,sizeof(vertices));
       BufferobjIndences.AddIndexBufferData(indices,sizeof(indices));

       VertexArrayobj.AddAtributePointer(0, 3, 6, 0);
       VertexArrayobj.AddAtributePointer(1, 3, 6, 3); 
      
       float PosX =0.0f, PosY =0.0f, PosZ= -3.0f;
       ImGui::CreateContext();
       ImGui_ImplGlfw_InitForOpenGL(MainWindow->MainWindow, true);
       ImGui::StyleColorsDark();
       Shader.UseShader();
       EditorCamera3D Camera;
       glm::vec3 Pos[] = { glm::vec3(1,3,4),glm::vec3(0.25,0.,27) };
        
        while ((glfwWindowShouldClose(CurrentWindow::GetWindow()) == false) && !(Input::IsKeyPressed(KeyBoardKey::Escape) ==true)) {
            float time = (float)glfwGetTime();
            Shader.UseShader();
            const FrameTime DeltaTime = time - LastFrameTime;
            LastFrameTime = time;

            glfwGetWindowSize(MainWindow->MainWindow, &MainWindow->Width, &MainWindow->Height); // this would be removed
            glm::mat4 Model= glm::mat4(1.0f);
            glm::mat4 View = glm::mat4(1.0f);
            glm::mat4 Projection = glm::mat4(1.0f);
            glm::mat4 Scale = glm::mat4(1.0f);

            Model = glm::rotate(Model, glm::radians(55.0f), glm::vec3(1.0f, 0, 0.0f));
            Model = glm::translate(Model, glm::vec3(PosX, PosY, PosZ));
            Projection = glm::perspective(glm::radians(Camera.GetFieldOfView()), (float)CurrentWindow::GetWindowWidth()/ (float)CurrentWindow::GetWindowHeight(), 0.1f, 100.0f);
            Scale = glm::scale(Scale, glm::vec3(5, 5, 3.4));
            Shader.SetMat4("Model", Model);
            Shader.SetMat4("Projection", Projection);
            Shader.SetMat4("View", View);
            Shader.SetMat4("Scale", Scale);
            Camera.OnUpdate(DeltaTime);
           /*
            if (Input::IsKeyPressed(KeyBoardKey::A)) {
                PosX -= 2 * DeltaTime;
            }
            
            if (Input::IsKeyPressed(KeyBoardKey::D)) {
                PosX += 2*DeltaTime;
            }
            if (Input::IsKeyPressed(KeyBoardKey::W)) {
                PosZ += 2 * DeltaTime;
            }

            if (Input::IsKeyPressed(KeyBoardKey::S)) {
                PosZ -= 2 * DeltaTime;
            }
            */
            
            VertexArrayobj.BindVertexArray();
           
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
            MainWindow->WindowUpdate(DeltaTime); 
        };
        MainWindow->WindowEnd();
        delete MainWindow;
    }
   
}