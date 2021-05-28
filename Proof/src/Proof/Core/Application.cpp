
#include "Proofprch.h"
#include "Application.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Platform/WindowsWindow.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof3D/Renderer/Camera/EditorCamera.h"

#include "Proof3D/Math/Vector.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <ImGUIOpenGL/imgui.h>
#include <ImGUIOpenGL/imgui_impl_glfw.h>
#include <ImGUIOpenGL/imgui_impl_opengl3.h>

#include "Platform/OpenGL/Shader.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLvertexArray.h"
#include "Platform/OpenGL/Texture.h"

#include "Platform/OpenGL/OpenGLGraphicsContext.h"
#include "Proof/Renderer/RendererCommand.h"
#include "Proof3D/Scene/Entity.h"
#include "Proof3D/Scene/World.h"
#include "Proof3D/Scene/ScriptableEntity.h"
#include "Proof3D/Scene/Component.h"
int GAMEPLAY = 1;
class SizePlayer:public  ScriptableEntity {
public:
    virtual void OnCreate() {
        
    }
    virtual void OnUpdate(FrameTime DeltaTime) {
        ScriptableEntity::OnUpdate(DeltaTime);
        if (Input::IsKeyPressed(KeyBoardKey::D) && Input::IsKeyPressed(KeyBoardKey::LeftShift))
            GetComponent<TransformComponent>()->Scale.X += 0.01;
        if (Input::IsKeyPressed(KeyBoardKey::A) && Input::IsKeyPressed(KeyBoardKey::LeftShift))
            GetComponent<TransformComponent>()->Scale.X -= 0.01;
    }
};

 class PlayerGG:public ScriptableEntity {
 public:
     virtual void OnCreate() {
     
     }
     virtual void OnUpdate(FrameTime DeltaTime) {
         if (Input::IsKeyPressed(KeyBoardKey::D)) {
             GetComponent<TransformComponent>()->Location.X += 0.00008f;
         }
         if (Input::IsKeyPressed(KeyBoardKey::A)) {
             GetComponent<TransformComponent>()->Location.X -= 0.00008f;
         }
     }
    
 };

namespace Proof {
   
   WindowsWindow* Application::MainWindow = nullptr;

   Application::Application() {
      MainWindow = new WindowsWindow(700,700);
      MainWindow->createWindow();

      ImGuiMainLayer = new ImGuiLayer();
      MainLayerStack.PushLayer(ImGuiMainLayer);
   }

   Application::~Application() {

   }

   void Application::Run() {
      RendererCommand::EnableDepth();
      PF_ENGINE_INFO("we are in editor mode");
      Shader LevelShader("Proof/Core/Level.shader",true);
      Shader LightCubeShader("Proof/Core/LightCube.shader",true);
      float vertices[] = {                       
       // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
      };
      glm::vec3 cubePositions[] = {
          glm::vec3(0.0f,0.0f,0.0f),
          glm::vec3(2.0f,5.0f,-15.0f),
          glm::vec3(-1.5f,-2.2f,-2.5f),
          glm::vec3(-3.8f,-2.0f,-12.3f),
          glm::vec3(2.4f,-0.4f,-3.5f),
          glm::vec3(-1.7f,3.0f,-7.5f),
          glm::vec3(1.3f,-2.0f,-2.5f),
          glm::vec3(1.5f,2.0f,-2.5f),
          glm::vec3(1.5f,0.2f,-1.5f),
          glm::vec3(-1.3f,1.0f,-1.5f)
      };
      OpenGLVertexArray VertexArrayobj;
      OpenGLVertexBuffer BufferObj;
      BufferObj.AddVertexBufferData(vertices,sizeof(vertices));
      BufferObj.BindVertexBuffer();
      VertexArrayobj.AddAtributePointerInt(0,3,8,0);
      VertexArrayobj.AddAtributePointerInt(1,3,8,3);
      VertexArrayobj.AddAtributePointerInt(2,2,8,6);

      OpenGLVertexArray LightCubeVerteArray;
      BufferObj.BindVertexBuffer();
      LightCubeVerteArray.AddAtributePointerInt(0,3,8,0);
      BufferObj.BindVertexBuffer();
      
      Texture DiffuseMap("Proof/Core/Container2.png");

      LevelShader.UseShader();
      LevelShader.SetInt("material.Diffuse",0);
      LevelShader.SetInt("material.Specular",1);
      float PosX = 0.0f,PosY = 0.0f,PosZ = -2.0f;
      EditorCamera3D Camera;

      Vector LightPosition(1.2f,1.0f,2.0f);
    //  Player1.AddComponent<NativeScriptComponent>()->Bind<RotationPlayer>();
      World* DefaultScene = new World();
      
      Entity Player1 = DefaultScene->CreateEntity();
      Player1.AddComponent<NativeScriptComponent>()->Bind<SizePlayer>();
      Player1.AddComponent<NativeScriptComponent>()->Bind<PlayerGG>();
      Player1.AddComponent<TransformComponent>();

      float Num1 =0.f, Num2= 0.f,Num3= 0.f;
      float FirstFrameTime = 0.0f;
      while ((glfwWindowShouldClose(CurrentWindow::GetWindow()) == false) && !(Input::IsKeyPressed(KeyBoardKey::Escape) == true)) {
         RendererCommand::Clear();
         RendererCommand::SetClearColor(0.1f,0.1f,0.1f,1.0f);
      
         float time = (float)glfwGetTime();
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
            for (Layer* layer : MainLayerStack.V_LayerStack)
               layer->OnUpdate(DeltaTime);
            if (Input::IsKeyClicked(KeyBoardKey::P)) {
                if (GAMEPLAY == 1) {
                     GAMEPLAY = 0;
                    PF_ENGINE_INFO("we are in Gameplay mode");
                }
                else if(GAMEPLAY ==0){
                     GAMEPLAY = 1;
                    PF_ENGINE_INFO("we are in Editor mode");
                    DefaultScene->EndRuntime();
                }
            }
            if (GAMEPLAY == 1) {
                Camera.OnUpdate(DeltaTime);
                DefaultScene->OnUpdateEditor(DeltaTime);
            }
            if (GAMEPLAY == 0) {
                DefaultScene->OnUpdateRuntime(DeltaTime);
            }
            LevelShader.UseShader();
            LevelShader.SetVec3("ViewPos",Camera.GetCameraPosition());
            LevelShader.SetVec3("light.direction",-0.2f,-1.0f,-0.3f);
            LevelShader.SetVec3("light.Ambient",0.2f,0.2f,0.2f);
            LevelShader.SetVec3("light.Diffuse",0.5f,0.5f,0.5f);
            LevelShader.SetVec3("light.Specular",1.0f,1.0f,1.0f);
            LevelShader.SetFloat("material.Shininess",256.0f);

            glm::mat4 View = Camera.GetCameraView();
            glm::mat4 Projection = glm::mat4(1.0f);
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::translate(Model,glm::vec3(Player1.GetComponent<TransformComponent>()->Location));
           // Model = glm::rotate(Model,glm::radians(90.0f),glm::vec3(Player1.GetRotation()));
            Model = glm::scale(Model,glm::vec3(Player1.GetComponent<TransformComponent>()->Scale));

         
            Projection = glm::perspective(glm::radians(Camera.GetFieldOfView()),(float)CurrentWindow::GetWindowWidth() / (float)CurrentWindow::GetWindowHeight(),0.1f,100.0f);
            LevelShader.UseShader();
            LevelShader.SetMat4("Projection",Projection);
            LevelShader.SetMat4("View",View);
            LevelShader.SetMat4("Model",Model);
            VertexArrayobj.BindVertexArray();
            DiffuseMap.BindTexture(0);
            glDrawArrays(GL_TRIANGLES,0,36);
            /*
            LevelShader.UseShader();
            LevelShader.SetVec3("ViewPos",Camera.GetCameraPosition());
            LevelShader.SetVec3("light.direction",-0.2f,-1.0f,-0.3f);
            LevelShader.SetVec3("light.Ambient",0.2f,0.2f,0.2f);
            LevelShader.SetVec3("light.Diffuse",0.5f,0.5f,0.5f);
            LevelShader.SetVec3("light.Specular",1.0f,1.0f,1.0f);
            LevelShader.SetFloat("material.Shininess",256.0f);
           
            glm::mat4 View = Camera.GetCameraView();
            glm::mat4 Projection = glm::mat4(1.0f);
            glm::mat4 Model = glm::mat4(1.0f);

            Projection = glm::perspective(glm::radians(Camera.GetFieldOfView()),(float)CurrentWindow::GetWindowWidth() / (float)CurrentWindow::GetWindowHeight(),0.1f,100.0f);
            LevelShader.UseShader();
            LevelShader.SetMat4("Projection",Projection);
            LevelShader.SetMat4("View",View);
            LevelShader.SetMat4("Model",Model);
          //  VertexArrayobj.BindVertexArray();

            DiffuseMap.BindTexture(0);
           // glDrawArrays(GL_TRIANGLES,0,36);
            for (unsigned int i = 0; i < 10; i++) {
                // calculate the model matrix for each object and pass it to shader before drawing
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model,cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model,glm::radians(angle),glm::vec3(1.0f,0.3f,0.5f));
                LevelShader.SetMat4("Model",model);
                VertexArrayobj.BindVertexArray();
                DiffuseMap.BindTexture(0);
                glDrawArrays(GL_TRIANGLES,0,36);
            }
            
            Model = glm::mat4(1.0f);
            Model = glm::translate(Model,glm::vec3(LightPosition));
            Model = glm::scale(Model,glm::vec3(0.2f));

            LightCubeShader.UseShader();
            LightCubeShader.SetMat4("Projection",Projection);
            LightCubeShader.SetMat4("View",View);
            LightCubeShader.SetMat4("Model",Model);
            LightCubeVerteArray.BindVertexArray();
            glDrawArrays(GL_TRIANGLES,0,36);
            */

            MainWindow->WindowUpdate(DeltaTime);
         }
         RendererCommand::SwapBuffer(CurrentWindow::GetWindow());
         RendererCommand::PollEvents();
         LastFrameTime = time;
      };
      MainWindow->WindowEnd();
      delete MainWindow;
   }

   void Application::PushLayer(Layer* Layer) {
      MainLayerStack.PushLayer(Layer);
   }

   void Application::PushOverlay(Layer* Layer) {
      MainLayerStack.PushOverlay(Layer);
   }
}