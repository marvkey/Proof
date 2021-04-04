#include "Proofprch.h"
#include "Application.h"
#include "Platform/WindowsWindow.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/OpenGL/Shader.h"
#include "Proof/Events/KeyEvent.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
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
       unsigned int VBO, VAO, EBO;
       glGenVertexArrays(1, &VAO);
       glGenBuffers(1, &VBO);
       glGenBuffers(1, &EBO);

       glBindVertexArray(VAO);

       glBindBuffer(GL_ARRAY_BUFFER, VBO);
       glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

       // position attribute
       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
       glEnableVertexAttribArray(0);
       // texture coord attribute
       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
       glEnableVertexAttribArray(1);
        
       float PosX =0.0f, PosY =0.0f, PosZ= -3.0f;
        Shader.UseShader();
        while ((glfwWindowShouldClose(CurrentWindow::GetWindow()) == false) && !(Input::IsKeyPressed(KeyBoardKey::Escape) ==true)) {

            float time = (float)glfwGetTime();
            const FrameTime DeltaTime = time - LastFrameTime;
            LastFrameTime = time;

            Shader.UseShader();

            glm::mat4 Model= glm::mat4(1.0f);
            glm::mat4 View = glm::mat4(1.0f);
            glm::mat4 Projection = glm::mat4(1.0f);

            Model = glm::rotate(Model, glm::radians(-45.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            View = glm::translate(View, glm::vec3(PosX, PosY, PosZ));
            Projection = glm::perspective(glm::radians(45.0f), (float)CurrentWindow::GetWindowWidth()/ (float)CurrentWindow::GetWindowHeight(), 0.1f, 100.0f);
            

            Shader.SetMat4("Model", Model);
            Shader.SetMat4("Projection", Projection);
            Shader.SetMat4("View", View);
           
            PF_ENGINE_INFO("{}", MainWindow->Width);
            if (Input::IsKeyPressed(KeyBoardKey::A)) {
                PosX -= 0.0001;
            }
            
            if (Input::IsKeyPressed(KeyBoardKey::D)) {
                PosX += 0.0001;
            }

            if (Input::IsKeyPressed(KeyBoardKey::W)) {
                PosY += 0.0001;
            }

            if (Input::IsKeyPressed(KeyBoardKey::S)) {
                PosY -= 0.0001;
            }
            
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            MainWindow->WindowUpdate(DeltaTime); 
        };
      
        
        MainWindow->WindowEnd();
        delete MainWindow;
    }
   
}