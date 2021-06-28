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
namespace Proof
{
    InstancedRenderer3D* Renderer3DInstance;
    Renderer3D::Data* Render3DData;
    static std::vector<glm::mat4> m_Transforms;
    void Renderer3D::Init() {
        Renderer3DInstance = new InstancedRenderer3D();
        Render3DData = new Renderer3D::Data;
        Renderer3DInstance->m_Shader = Shader::Create("InstanceMeshRenderer",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/MeshShader.shader");
        Renderer3DInstance->m_VertexBuffer = VertexBuffer::Create(Render3DData->MaxMeshes * sizeof(glm::mat4));
        //Renderer3DInstance->m_VertexArray = VertexArray::Create();
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
            for (unsigned int i = 0; i < meshComponent.GetModel()->GetMesh().size(); i++) {
                meshComponent.GetModel()->GetMesh()[i].VertexArrayObject->Bind();
                meshComponent.GetModel()->GetMesh()[i].VertexArrayObject->AddData(3,4,sizeof(glm::mat4),(void*)0);
                meshComponent.GetModel()->GetMesh()[i].VertexArrayObject->AddData(4,4,sizeof(glm::mat4),(void*)(sizeof(glm::vec4)));

                meshComponent.GetModel()->GetMesh()[i].VertexArrayObject->AddData(5,4,sizeof(glm::mat4),(void*)(2 * sizeof(glm::vec4)));
                meshComponent.GetModel()->GetMesh()[i].VertexArrayObject->AddData(6,4,sizeof(glm::mat4),(void*)(3 * sizeof(glm::vec4)));
                meshComponent.GetModel()->GetMesh()[i].VertexArrayObject->Bind();
                glVertexAttribDivisor(3,1);
                glVertexAttribDivisor(4,1);
                glVertexAttribDivisor(5,1);
                glVertexAttribDivisor(6,1);
                glm::mat4 ModelMatrix = glm::mat4(1.0f);
                ModelMatrix = glm::translate(ModelMatrix,{meshComponent.Transform.Location});
                ModelMatrix = glm::scale(ModelMatrix,{meshComponent.Transform.Scale});
                m_Transforms.emplace_back(ModelMatrix);

                if (meshComponent.GetModel()->GetMesh().size() == 1)
                    break;
            }
        }
        else {

            PF_ENGINE_INFO("New Created");
            Renderer3DInstance->m_AmountMeshes.insert({meshComponent.GetID(),1});
            Renderer3DInstance->m_Meshes.insert({meshComponent.GetID(),meshComponent});

            for (unsigned int i = 0; i < meshComponent.GetModel()->GetMesh().size(); i++) {

                meshComponent.GetModel()->GetMesh()[i].VertexArrayObject->Bind();
                meshComponent.GetModel()->GetMesh()[i].VertexArrayObject->AddData(3,4,sizeof(glm::mat4),(void*)0);
                meshComponent.GetModel()->GetMesh()[i].VertexArrayObject->AddData(4,4,sizeof(glm::mat4),(void*)(sizeof(glm::vec4)));

                meshComponent.GetModel()->GetMesh()[i].VertexArrayObject->AddData(5,4,sizeof(glm::mat4),(void*)(2 * sizeof(glm::vec4)));
                meshComponent.GetModel()->GetMesh()[i].VertexArrayObject->AddData(6,4,sizeof(glm::mat4),(void*)(3 * sizeof(glm::vec4)));

                meshComponent.GetModel()->GetMesh()[i].VertexArrayObject->AttributeDivisor(3,1);
                meshComponent.GetModel()->GetMesh()[i].VertexArrayObject->AttributeDivisor(4,1);
                meshComponent.GetModel()->GetMesh()[i].VertexArrayObject->AttributeDivisor(5,1);
                meshComponent.GetModel()->GetMesh()[i].VertexArrayObject->AttributeDivisor(6,1);

              
                glm::mat4 ModelMatrix = glm::mat4(1.0f);
                ModelMatrix = glm::translate(ModelMatrix,{meshComponent.Transform.Location});
                ModelMatrix = glm::scale(ModelMatrix,{meshComponent.Transform.Scale});
                m_Transforms.emplace_back(ModelMatrix);

                if(meshComponent.GetModel()->GetMesh().size() ==1)
                    break;
            }
        }

        PF_ENGINE_INFO("DONE  Setting Up New Created");
    }
    void Renderer3D::EndContext() {
        Renderer3DInstance->m_Shader->UseShader();
        PF_ENGINE_INFO("Start Render");
        Renderer3DInstance->m_VertexBuffer->Bind();
        Renderer3DInstance->m_VertexBuffer->AddData(&m_Transforms[0],m_Transforms.size() * sizeof(glm::mat4));
        for (unsigned int i = 0; i < Renderer3DInstance->m_Meshes.find(1)->second.GetModel()->GetMesh().size(); i++) {
            Renderer3DInstance->m_Meshes.find(1)->second.GetModel()->GetMesh()[i].Draw(Renderer3DInstance->m_Shader);
            Renderer3DInstance->m_Meshes.find(1)->second.GetModel()->GetMesh()[i].VertexArrayObject->Bind();
            glDrawElementsInstanced(GL_TRIANGLES,Renderer3DInstance->m_Meshes.find(1)->second.GetModel()->GetMesh()[i].m_Indices.size(),GL_UNSIGNED_INT,0,1);
        }
        PF_ENGINE_INFO("Done Render");
        Renderer3DInstance->m_AmountMeshes.clear();
        Renderer3DInstance->m_Meshes.clear();
        m_Transforms.clear();
    }
}