#include "Proofprch.h"

#include "Renderer3D.h"
#include "Shader.h"
//#include "Platform/Window/CurrentWindow.h"
#include "VertexArray.h"
#include "Renderer.h"

#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Camera/PerspectiveCamera.h"
#include "Proof/Scene/Camera/OrthagraphicCamera.h"
#include "Proof/Scene/Camera/EditorCamera.h"

#include "Proof/Scene/Model.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Material.h"
namespace Proof
{
    static Material EmptyMaterial;
    static uint32_t Temp2 = (sizeof(glm::vec4));
    static uint32_t Temp3 = (sizeof(glm::vec4) * 2);
    static uint32_t Temp4 = (sizeof(glm::vec4) * 3);
    static uint32_t Temp5 = (sizeof(glm::vec4) * 4);
    static glm::vec3 Position;
    struct InstanceRendererVertex{
        InstanceRendererVertex(const glm::mat4& Transform,const Material& mat)
            :
            m_Transform(Transform)
        {
            m_MaterialAmbient = mat.m_Ambient;
            m_MaterialDiffuse = mat.m_Diuffuse;
            m_MaterialSpecular = mat.m_Specular;
            m_Matallness= mat.m_Metallness;
        }
        glm::mat4 m_Transform;
        Vector m_MaterialAmbient;
        Vector m_MaterialDiffuse;
        Vector m_MaterialSpecular;
        float m_Matallness =1.f; // default is one
    };
    static InstancedRenderer3D* Renderer3DInstance;
    Renderer3D::Render3DStats* Renderer3DStats;
    static std::vector<glm::mat4> m_Transforms;
    static std::vector<glm::vec4> m_Material;
    static std::vector<uint32_t> s_DifferentID;
    static uint32_t DifferentMeshes = 0;
    static glm::mat4 ModelMatrix; 

    static std::vector<InstanceRendererVertex> m_InstanceTransforms;

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
        //Renderer3DInstance->m_Shader->SetVec3("viewPos",Camera.Pos);

    }
    void Renderer3D::BeginContext(glm::mat4 Projection,EditorCamera3D& EditorCamera) {
        Renderer3DInstance->m_Shader->SetMat4("u_Projection",Projection);
        Renderer3DInstance->m_Shader->SetMat4("u_View",EditorCamera.GetCameraView());
        Position = EditorCamera.GetCameraPosition();
    }
    void Renderer3D::BeginContext(const OrthagraphicCamera& Camera) {
        Renderer3DInstance->m_Shader->UseShader();
        Renderer3DInstance->m_Shader->SetMat4("u_Projection",Camera.GetProjectionMatrix());
        Renderer3DInstance->m_Shader->SetMat4("u_View",Camera.GetViewMatrix());
    }
    void Renderer3D::Draw(MeshComponent& meshComponent) {

        if (Renderer3DInstance->SceneHasAmountMeshes(meshComponent.GetMeshPointerID()) == true) {
            auto Map = Renderer3DInstance->m_AmountMeshes.find(meshComponent.GetMeshPointerID());
            Map->second += 1;

            meshComponent.GetModel()->m_VertexArrayObject->Bind();

            meshComponent.GetModel()->m_VertexArrayObject->AddData(3,4,sizeof(InstanceRendererVertex),(void*)offsetof(InstanceRendererVertex,m_Transform));
            meshComponent.GetModel()->m_VertexArrayObject->AddData(4,4,sizeof(InstanceRendererVertex),(void*)Temp2);
            meshComponent.GetModel()->m_VertexArrayObject->AddData(5,4,sizeof(InstanceRendererVertex),(void*)Temp3);
            meshComponent.GetModel()->m_VertexArrayObject->AddData(6,4,sizeof(InstanceRendererVertex),(void*)Temp4);
            meshComponent.GetModel()->m_VertexArrayObject->AddData(7,3,sizeof(InstanceRendererVertex),(void*)offsetof(InstanceRendererVertex,InstanceRendererVertex::m_MaterialAmbient));
            meshComponent.GetModel()->m_VertexArrayObject->AddData(8,3,sizeof(InstanceRendererVertex),(void*)offsetof(InstanceRendererVertex,InstanceRendererVertex::m_MaterialDiffuse));
            meshComponent.GetModel()->m_VertexArrayObject->AddData(9,3,sizeof(InstanceRendererVertex),(void*)offsetof(InstanceRendererVertex,InstanceRendererVertex::m_MaterialSpecular));
            meshComponent.GetModel()->m_VertexArrayObject->AddData(10,1,sizeof(InstanceRendererVertex),(void*)offsetof(InstanceRendererVertex,InstanceRendererVertex::m_Matallness));


            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(3,1);
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(4,1);
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(5,1);
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(6,1);
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(7,1);// Material
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(8,1);// Material
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(9,1);// Material
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(10,1);// Material

            ModelMatrix = glm::mat4(1.0f);
            auto Transform = meshComponent.GetOwner().GetComponent<TransformComponent>();
            ModelMatrix = glm::translate(ModelMatrix,{Transform->Location + meshComponent.MeshLocalTransform.Location});
            ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform->Rotation.X + meshComponent.MeshLocalTransform.Rotation.X),{1,0,0});
            ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform->Rotation.Y + meshComponent.MeshLocalTransform.Rotation.Y),{0,1,0});
            ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform->Rotation.Z + meshComponent.MeshLocalTransform.Rotation.Z),{0,0,1});
            ModelMatrix = glm::scale(ModelMatrix,{Transform->Scale + meshComponent.MeshLocalTransform.Scale});

            InstanceRendererVertex temp(ModelMatrix,meshComponent.HasMaterial() == true ? *meshComponent.GetMaterial() : EmptyMaterial);
            m_InstanceTransforms.emplace_back(temp);
            meshComponent.GetModel()->m_VertexArrayObject->AddData(10,1,sizeof(InstanceRendererVertex),(void*)offsetof(InstanceRendererVertex,InstanceRendererVertex::m_Matallness));
            Renderer3DStats->AmountDrawn += 1;
        }
        else {
                Renderer3DInstance->m_AmountMeshes.insert({meshComponent.GetMeshPointerID(),1});
                Renderer3DInstance->m_Meshes.insert({meshComponent.GetMeshPointerID(),meshComponent});
                s_DifferentID.emplace_back(meshComponent.GetMeshPointerID());
                DifferentMeshes++;


            meshComponent.GetModel()->m_VertexArrayObject->Bind();

            meshComponent.GetModel()->m_VertexArrayObject->AddData(3,4,sizeof(InstanceRendererVertex),(void*)offsetof(InstanceRendererVertex,m_Transform));
            meshComponent.GetModel()->m_VertexArrayObject->AddData(4,4,sizeof(InstanceRendererVertex),(void*)Temp2);
            meshComponent.GetModel()->m_VertexArrayObject->AddData(5,4,sizeof(InstanceRendererVertex),(void*)Temp3);
            meshComponent.GetModel()->m_VertexArrayObject->AddData(6,4,sizeof(InstanceRendererVertex),(void*)Temp4);
            meshComponent.GetModel()->m_VertexArrayObject->AddData(7,3,sizeof(InstanceRendererVertex),(void*)offsetof(InstanceRendererVertex,InstanceRendererVertex::m_MaterialAmbient));
            meshComponent.GetModel()->m_VertexArrayObject->AddData(8,3,sizeof(InstanceRendererVertex),(void*)offsetof(InstanceRendererVertex,InstanceRendererVertex::m_MaterialDiffuse));
            meshComponent.GetModel()->m_VertexArrayObject->AddData(9,3,sizeof(InstanceRendererVertex),(void*)offsetof(InstanceRendererVertex,InstanceRendererVertex::m_MaterialSpecular));
            meshComponent.GetModel()->m_VertexArrayObject->AddData(10,1,sizeof(InstanceRendererVertex),(void*)offsetof(InstanceRendererVertex,InstanceRendererVertex::m_Matallness));
              


            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(3,1);
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(4,1);
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(5,1);
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(6,1);
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(7,1);// Material
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(8,1);// Material
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(9,1);// MaterialMaterial
            meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(10,1);// MaterialMaterial
              
            ModelMatrix = glm::mat4(1.0f);
            auto Transform = meshComponent.GetOwner().GetComponent<TransformComponent>();
            ModelMatrix = glm::translate(ModelMatrix,{Transform->Location + meshComponent.MeshLocalTransform.Location});
            ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform->Rotation.X + meshComponent.MeshLocalTransform.Rotation.X),{1,0,0});
            ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform->Rotation.Y + meshComponent.MeshLocalTransform.Rotation.Y),{0,1,0});
            ModelMatrix = glm::rotate(ModelMatrix,glm::radians(Transform->Rotation.Z + meshComponent.MeshLocalTransform.Rotation.Z),{0,0,1});
            ModelMatrix = glm::scale(ModelMatrix,{Transform->Scale + meshComponent.MeshLocalTransform.Scale});

            InstanceRendererVertex temp(ModelMatrix,meshComponent.HasMaterial() ==true ? *meshComponent.GetMaterial(): EmptyMaterial);

            m_InstanceTransforms.emplace_back(temp);
            Renderer3DStats->AmountDrawn += 1;
            Renderer3DStats->NumberOfInstances += 1;
        }
    }
    void Renderer3D::EndContext() {
        if (DifferentMeshes == 0)return;
        uint32_t SizeofOffset = 0;
        Renderer3DInstance->m_Shader->UseShader();
        Renderer3DInstance->m_Shader->SetVec3("light.ambient",0.2f,0.2f,0.2f);
        Renderer3DInstance->m_Shader->SetVec3("light.diffuse",0.5f,0.5f,0.5f);
        Renderer3DInstance->m_Shader->SetVec3("light.specular",1.0f,1.0f,1.0f);
        Renderer3DInstance->m_Shader->SetVec3("viewPos",Position);
        for (uint32_t Size = 0; Size < s_DifferentID.size(); Size++) {
            Renderer3DInstance->m_VertexBuffer->Bind();
           //Renderer3DInstance->m_VertexBuffer->AddData(&m_Material[SizeofOffset],m_Material.size() * sizeof(glm::vec4));/* THIS CODE COULD BE TEMPORARY*/
            Renderer3DInstance->m_VertexBuffer->AddData(&m_InstanceTransforms[SizeofOffset],m_InstanceTransforms.size() * sizeof(InstanceRendererVertex));/* THIS CODE COULD BE TEMPORARY*/
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
        m_Material.clear();
        m_InstanceTransforms.clear();
        DifferentMeshes = 0;
        Renderer3DStats->DrawCalls = 0;
        Renderer3DStats->AmountDrawn = 0;
        Renderer3DStats->NumberOfInstances = 0;
    }
}