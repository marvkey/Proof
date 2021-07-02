#include "Proofprch.h"

#include "Renderer3D.h"
#include "Shader.h"
//#include "Platform/Window/CurrentWindow.h"
#include "VertexArray.h"
#include "Renderer.h"

#include "Proof3D/Scene/Mesh.h"
#include "Proof3D/Scene/Component.h"
#include "Proof3D/Renderer/Camera/PerspectiveCamera.h"
#include "Proof3D/Renderer/Camera/OrthagraphicCamera.h"
#include "Proof3D/Renderer/Camera/EditorCamera.h"

#include "Proof3D/Scene/Model.h"
#include "Proof/Core/FrameTime.h"
#include "Proof3D/Scene/Mesh.h"
#include "Proof3D/Scene/Entity.h"
namespace Proof{
    static InstancedRenderer3D* Renderer3DInstance;
    Renderer3D::Data* Render3DData;
    static std::vector<glm::mat4> m_Transforms;
    static std::vector<uint32_t> s_DifferentID;
    static uint32_t DifferentMeshes = 0;
    void Renderer3D::Init() {
        Renderer3DInstance = new InstancedRenderer3D();
        Render3DData = new Renderer3D::Data;
        Renderer3DInstance->m_Shader = Shader::Create("InstanceMeshRenderer",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/MeshShader.shader");
        Renderer3DInstance->m_VertexBuffer = VertexBuffer::Create(Render3DData->MaxMeshes * sizeof(glm::mat4));
    }
    void Renderer3D::BeginContext(const PerspectiveCamera& Camera) {
        Renderer3DInstance->m_Shader->UseShader();
        Renderer3DInstance->m_Shader->SetMat4("u_Projection",Camera.GetProjectionMatrix());
        Renderer3DInstance->m_Shader->SetMat4("u_View",Camera.GetViewMatrix());
    }
    void Renderer3D::BeginContext(glm::mat4 Projection,EditorCamera3D& EditorCamera) {
        Renderer3DInstance->m_Shader->SetMat4("u_Projection",Projection);
        Renderer3DInstance->m_Shader->SetMat4("u_View",EditorCamera.GetCameraView());
    }
    void Renderer3D::BeginContext(const OrthagraphicCamera& Camera) {
        Renderer3DInstance->m_Shader->UseShader();
        Renderer3DInstance->m_Shader->SetMat4("u_Projection",Camera.GetProjectionMatrix());
        Renderer3DInstance->m_Shader->SetMat4("u_View",Camera.GetViewMatrix());
    }
    void Renderer3D::Draw(MeshComponent& meshComponent) {
        if (Renderer3DInstance->SceneHasAmountMeshes(meshComponent.GetID()) == true) {
            auto Map = Renderer3DInstance->m_AmountMeshes.find(meshComponent.GetID());
            Map->second += 1;

            meshComponent.GetModel()->m_VertexArrayObject->Bind();
            meshComponent.GetModel()->m_VertexArrayObject->AddData(3,4,sizeof(glm::mat4),(void*)0);
            meshComponent.GetModel()->m_VertexArrayObject->AddData(4,4,sizeof(glm::mat4),(void*)(sizeof(glm::vec4)));

            meshComponent.GetModel()->m_VertexArrayObject->AddData(5,4,sizeof(glm::mat4),(void*)(2 * sizeof(glm::vec4)));
            meshComponent.GetModel()->m_VertexArrayObject->AddData(6,4,sizeof(glm::mat4),(void*)(3 * sizeof(glm::vec4)));

            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(3,1);
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(4,1);
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(5,1);
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(6,1);

            glm::mat4 ModelMatrix = glm::mat4(1.0f);
            
            ModelMatrix = glm::translate(ModelMatrix,{meshComponent.GetOwner()->GetComponent<TransformComponent>()->Location});
            ModelMatrix = glm::scale(ModelMatrix,{meshComponent.GetOwner()->GetComponent<TransformComponent>()->Scale});
            m_Transforms.emplace_back(ModelMatrix);
        }
        else {

            // PF_ENGINE_INFO("New Created");
            Renderer3DInstance->m_AmountMeshes.insert({meshComponent.GetID(),1});
            Renderer3DInstance->m_Meshes.insert({meshComponent.GetID(),meshComponent});
            s_DifferentID.emplace_back(meshComponent.GetID());
            DifferentMeshes++;

            meshComponent.GetModel()->m_VertexArrayObject->Bind();
            meshComponent.GetModel()->m_VertexArrayObject->AddData(3,4,sizeof(glm::mat4),(void*)0);
            meshComponent.GetModel()->m_VertexArrayObject->AddData(4,4,sizeof(glm::mat4),(void*)(sizeof(glm::vec4)));

            meshComponent.GetModel()->m_VertexArrayObject->AddData(5,4,sizeof(glm::mat4),(void*)(2 * sizeof(glm::vec4)));
            meshComponent.GetModel()->m_VertexArrayObject->AddData(6,4,sizeof(glm::mat4),(void*)(3 * sizeof(glm::vec4)));

            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(3,1);
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(4,1);
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(5,1);
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(6,1);

            glm::mat4 ModelMatrix = glm::mat4(1.0f);
            auto Transform = meshComponent.GetOwner()->GetComponent<TransformComponent>();
            ModelMatrix = glm::translate(ModelMatrix,{Transform->Location});
            ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform->Rotation.X),{1,0,0});
            ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform->Rotation.Y),{0,1,0});
            ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform->Rotation.Z),{0,0,1});
            ModelMatrix = glm::scale(ModelMatrix,{Transform->Scale});
            m_Transforms.emplace_back(ModelMatrix);
            //PF_ENGINE_INFO("DONE  Setting Up New Created");
        }
    }
    void Renderer3D::EndContext() {
        //PF_ENGINE_INFO("Start Render");
        Renderer3DInstance->m_VertexBuffer->Bind();
        Renderer3DInstance->m_VertexBuffer->AddData(&m_Transforms[0],m_Transforms.size() * sizeof(glm::mat4));
        /*
        //Renderer3DInstance->m_Meshes.find(1)->second.GetModel().Draw(Renderer3DInstance->m_Shader);

        //Renderer3DInstance->m_Shader->UseShader();
        Renderer3DInstance->m_Shader->SetInt("texture_diffuse",0);
        Renderer3DInstance->m_Meshes.find(1)->second.GetModel()->textures_loaded[0]->BindTexture(0);
        Renderer3DInstance->m_Meshes.find(1)->second.GetModel()->m_VertexArrayObject->Bind();
        Renderer3DInstance->m_Meshes.find(1)->second.GetModel()->m_IndexBufferObject->Bind();
        glDrawElementsInstanced(GL_TRIANGLES,Renderer3DInstance->m_Meshes.find(1)->second.GetModel()->m_IndexBufferObject->GetCount(),GL_UNSIGNED_INT,0,1);
        PF_ENGINE_INFO("Done Render");
        */

        for(uint32_t AmountDifferentMeshes =0; AmountDifferentMeshes <= DifferentMeshes;AmountDifferentMeshes++){
            for(uint32_t Size=0; Size <= s_DifferentID.size(); Size++){

                uint32_t TempID = s_DifferentID[Size];

                Renderer3DInstance->m_Shader->UseShader();
                Renderer3DInstance->m_Shader->SetInt("texture_diffuse",0);
                Renderer3DInstance->m_Meshes.find(TempID)->second.GetModel()->textures_loaded[0]->BindTexture(0);
                Renderer3DInstance->m_Meshes.find(TempID)->second.GetModel()->m_VertexArrayObject->Bind();
                Renderer3DInstance->m_Meshes.find(TempID)->second.GetModel()->m_IndexBufferObject->Bind();
                RendererCommand::DrawElementIndexed(Renderer3DInstance->m_Meshes.find(TempID)->second.GetModel()->m_VertexArrayObject,Renderer3DInstance->m_AmountMeshes.find(TempID)->second);
                if (s_DifferentID.size() == 1)break;
            }
        }
        //PF_ENGINE_INFO("Done Render");
        Renderer3DInstance->m_AmountMeshes.clear();
        Renderer3DInstance->m_Meshes.clear();
        m_Transforms.clear();
        s_DifferentID.clear();
        DifferentMeshes =0;
    }
}