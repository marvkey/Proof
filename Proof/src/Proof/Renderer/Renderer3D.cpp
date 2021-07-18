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
namespace Proof
{
    static InstancedRenderer3D* Renderer3DInstance;
    Renderer3D::Render3DStats* Renderer3DStats;
    static std::vector<glm::mat4> m_Transforms;
    static std::vector<uint32_t> s_DifferentID;
    static uint32_t DifferentMeshes = 0;
    static glm::mat4 ModelMatrix;

    uint32_t Renderer3D::Render3DStats::DrawCalls = 0;
    uint32_t Renderer3D::Render3DStats::NumberOfInstances = 0;
    uint32_t Renderer3D::Render3DStats::AmountDrawn = 0;
    Count<Texture2D>InstancedRenderer3D::m_WhiteTexture;
    void Renderer3D::Init() {
        Renderer3DInstance = new InstancedRenderer3D();
        Renderer3DStats = new Renderer3D::Render3DStats;
        Renderer3DInstance->m_Shader = Shader::Create("InstanceMeshRenderer",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/MeshShader.shader");
        Renderer3DInstance->m_VertexBuffer = VertexBuffer::Create(Renderer3DStats->MaxMeshes * sizeof(glm::mat4));
        Renderer3DInstance->m_WhiteTexture = Texture2D::Create(1,1);
        uint32_t WhiteTextureImage = 0xffffffff;
        Renderer3DInstance->m_WhiteTexture->SetData(&WhiteTextureImage,sizeof(uint32_t));
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
            ModelMatrix = glm::mat4(1.0f);

            auto Transform = meshComponent.GetOwner().GetComponent<TransformComponent>();
            ModelMatrix = glm::translate(ModelMatrix,{Transform->Location + meshComponent.MeshLocalTransform.Location});
            ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform->Rotation.X + meshComponent.MeshLocalTransform.Rotation.X),{1,0,0});
            ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform->Rotation.Y + meshComponent.MeshLocalTransform.Rotation.Y),{0,1,0});
            ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform->Rotation.Z + meshComponent.MeshLocalTransform.Rotation.Z),{0,0,1});
            ModelMatrix = glm::scale(ModelMatrix,{Transform->Scale + meshComponent.MeshLocalTransform.Scale});
            m_Transforms.emplace_back(ModelMatrix);
            Renderer3DStats->AmountDrawn += 1;
        }
        else {
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

            ModelMatrix = glm::mat4(1.0f);
            auto Transform = meshComponent.GetOwner().GetComponent<TransformComponent>();
            ModelMatrix = glm::translate(ModelMatrix,{Transform->Location + meshComponent.MeshLocalTransform.Location});
            ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform->Rotation.X + meshComponent.MeshLocalTransform.Rotation.X),{1,0,0});
            ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform->Rotation.Y + meshComponent.MeshLocalTransform.Rotation.Y),{0,1,0});
            ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform->Rotation.Z + meshComponent.MeshLocalTransform.Rotation.Z),{0,0,1});
            ModelMatrix = glm::scale(ModelMatrix,{Transform->Scale + meshComponent.MeshLocalTransform.Scale});
            m_Transforms.emplace_back(ModelMatrix);
            Renderer3DStats->AmountDrawn += 1;
            Renderer3DStats->NumberOfInstances += 1;
        }
    }
    void Renderer3D::EndContext() {
        if (DifferentMeshes == 0)return;
        PF_PROFILE_FUNCTION();
        uint32_t SizeofOffset = 0;
        for (uint32_t Size = 0; Size <= s_DifferentID.size()-1; Size++) {
            Renderer3DInstance->m_VertexBuffer->Bind();
            Renderer3DInstance->m_VertexBuffer->AddData(&m_Transforms[SizeofOffset],m_Transforms.size() * sizeof(glm::mat4));/* THIS CODE COULD BE TEMPORARY*/
            uint32_t TempID = s_DifferentID[Size];
            auto& TempMesh = Renderer3DInstance->m_Meshes.find(TempID);
            auto& TempAmountMeshes = Renderer3DInstance->m_AmountMeshes.find(TempID);
            if (TempMesh->second.GetModel()->textures_loaded.size() > 0) {
                Renderer3DInstance->m_Shader->UseShader();
                Renderer3DInstance->m_Shader->SetInt("texture_diffuse",0);
                TempMesh->second.GetModel()->textures_loaded[0]->BindTexture(0);
            }
            else {
                Renderer3DInstance->m_Shader->UseShader();
                Renderer3DInstance->m_Shader->SetInt("texture_diffuse",0);
                Renderer3DInstance->m_WhiteTexture->BindTexture(0);
            }
            TempMesh->second.GetModel()->m_VertexArrayObject->Bind();
            TempMesh->second.GetModel()->m_IndexBufferObject->Bind();
            RendererCommand::DrawElementIndexed(TempMesh->second.GetModel()->m_VertexArrayObject,TempAmountMeshes->second);
            Renderer3DStats->DrawCalls++;
            SizeofOffset+= TempAmountMeshes->second;
        }
    }

    void Renderer3D::Reset() {
        Renderer3DInstance->m_AmountMeshes.clear();
        Renderer3DInstance->m_Meshes.clear();
        m_Transforms.clear();
        s_DifferentID.clear();
        DifferentMeshes = 0;
        Renderer3DStats->DrawCalls = 0;
        Renderer3DStats->AmountDrawn = 0;
        Renderer3DStats->NumberOfInstances = 0;
    }
}