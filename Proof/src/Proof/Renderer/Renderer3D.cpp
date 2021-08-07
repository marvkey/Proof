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

/* RIGHT NOW ONLY SUPPORT 150 directional Light*/
/* RIGHT NOW ONLY SUPPORT 150 Diffuse Light*/
/* RIGHT NOW ONLY SUPPORT 150 Specular Light*/
namespace Proof
{
    static uint32_t NumberDirectionalLight=0;
    static uint32_t NumberPointLight =0;
    static uint32_t NumberSpotLight=0;

    std::string NumberDirectionalLightstring;
    std::string NumberPointLightstring;
    std::string NumberSpotLightstring;

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
            auto& Map = Renderer3DInstance->m_AmountMeshes.find(meshComponent.GetMeshPointerID());
            Map->second += 1;
            auto InstanceSize = Renderer3DInstance->m_MeshesEndingPositionIndexTransforms.find(meshComponent.GetMeshPointerID());
            meshComponent.GetModel()->m_VertexArrayObject->Bind();

            SetMeshComponentData(meshComponent);

            InstanceRendererVertex temp(ModelMatrix,meshComponent.HasMaterial() == true ? *meshComponent.GetMaterial() : EmptyMaterial);
            m_InstanceTransforms.insert(m_InstanceTransforms.begin()+ InstanceSize->second,temp);
            InstanceSize->second += 1;
            Renderer3DStats->AmountDrawn += 1;
        }
        else 
        {
            Renderer3DInstance->m_AmountMeshes.insert({meshComponent.GetMeshPointerID(),1});
            Renderer3DInstance->m_Meshes.insert({meshComponent.GetMeshPointerID(),meshComponent});
            Renderer3DInstance->m_MeshesEndingPositionIndexTransforms.insert({meshComponent.GetMeshPointerID(),m_InstanceTransforms.size()+1});
            s_DifferentID.emplace_back(meshComponent.GetMeshPointerID());
            DifferentMeshes++;

            SetMeshComponentData(meshComponent);

            InstanceRendererVertex temp(ModelMatrix,meshComponent.HasMaterial() ==true ? *meshComponent.GetMaterial(): EmptyMaterial);
            m_InstanceTransforms.emplace_back(temp);
            Renderer3DStats->AmountDrawn += 1;
            Renderer3DStats->NumberOfInstances += 1;
        }
    }
    void Renderer3D::RenderLight(LightComponent& lightComponent) {
        if(lightComponent.m_LightType == lightComponent.Direction&& NumberDirectionalLight<150){
            NumberDirectionalLightstring = "v_DirectionalLight[" + std::to_string(NumberDirectionalLight) + "]";
            Renderer3DInstance->m_Shader->UseShader();
            Renderer3DInstance->m_Shader->SetVec3(NumberDirectionalLightstring + ".direction",lightComponent.m_Direction);
            Renderer3DInstance->m_Shader->SetVec3(NumberDirectionalLightstring + ".ambient",lightComponent.m_Ambient);
            Renderer3DInstance->m_Shader->SetVec3(NumberDirectionalLightstring + ".diffuse",lightComponent.m_Diffuse);
            Renderer3DInstance->m_Shader->SetVec3(NumberDirectionalLightstring + ".specular",lightComponent.m_Specular);
            NumberDirectionalLight++;
            Renderer3DInstance->m_Shader->SetInt("v_NrDirectionalLight",NumberDirectionalLight);
            
            return;
        }

        if(lightComponent.m_LightType ==lightComponent.Point&& NumberPointLight<150){
            NumberPointLightstring = "v_PointLight[" + std::to_string(NumberPointLight) + "]";
            Renderer3DInstance->m_Shader->UseShader();
            Renderer3DInstance->m_Shader->SetVec3(NumberPointLightstring + ".position",lightComponent.m_Position+lightComponent.GetOwner().GetComponent<TransformComponent>()->Location);
            Renderer3DInstance->m_Shader->SetVec3(NumberPointLightstring + ".ambient",lightComponent.m_Ambient);
            Renderer3DInstance->m_Shader->SetVec3(NumberPointLightstring + ".diffuse",lightComponent.m_Diffuse);
            Renderer3DInstance->m_Shader->SetVec3(NumberPointLightstring + ".specular",lightComponent.m_Specular);
            Renderer3DInstance->m_Shader->SetFloat(NumberPointLightstring + ".constant",lightComponent.m_Constant);
            Renderer3DInstance->m_Shader->SetFloat(NumberPointLightstring + ".linear",lightComponent.m_Linear);
            Renderer3DInstance->m_Shader->SetFloat(NumberPointLightstring + ".quadratic",lightComponent.m_Quadratic);
            NumberPointLight++;
            Renderer3DInstance->m_Shader->SetInt("v_NrPointLight",NumberPointLight);
            return;
        }

        if(lightComponent.m_LightType ==lightComponent.Spot && NumberPointLight<150){
            NumberSpotLightstring = "v_SpotLight[" + std::to_string(NumberSpotLight) + "]";
            Renderer3DInstance->m_Shader->UseShader();
            Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".direction",lightComponent.m_Direction);
            Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".position",{lightComponent.m_Position + lightComponent.GetOwner().GetComponent<TransformComponent>()->Location});
            Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".ambient",lightComponent.m_Ambient);
            Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".diffuse",lightComponent.m_Diffuse);
            Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".specular",lightComponent.m_Specular);
            Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".constant",lightComponent.m_Constant);
            Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".linear",lightComponent.m_Linear);
            Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".quadratic",lightComponent.m_Quadratic);
            Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".cutOff",Math::Cos(Math::Radian(lightComponent.m_CutOff)));
            Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".outerCutOff",Math::Cos(Math::Radian(lightComponent.m_OuterCutOff)));
            NumberSpotLight++;
            Renderer3DInstance->m_Shader->SetInt("v_NrSpotLight",NumberSpotLight);
            return;

        }
    }
    void Renderer3D::EndContext() {
        if (DifferentMeshes == 0)return;
        uint32_t SizeofOffset = 0;
        Renderer3DInstance->m_Shader->UseShader();
        LightingErrorChecks();
        Renderer3DInstance->m_Shader->SetVec3("viewPos",Position);
        for (uint32_t Size = 0; Size < s_DifferentID.size(); Size++) {
            Renderer3DInstance->m_VertexBuffer->Bind();
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
                Renderer3DInstance->m_Shader->SetInt( "texture_diffuse",0);
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
        Renderer3DInstance->m_MeshesEndingPositionIndexTransforms.clear();
        m_Transforms.clear();
        s_DifferentID.clear();
        m_Material.clear();
        m_InstanceTransforms.clear();
        DifferentMeshes = 0;
        Renderer3DStats->DrawCalls = 0;          
        Renderer3DStats->AmountDrawn = 0;
        Renderer3DStats->NumberOfInstances = 0;

        NumberDirectionalLight =0;
        NumberPointLight=0;
        NumberSpotLight =0;
        /*
        NumberDirectionalLightstring = " ";
        NumberPointLightstring =" ";
        NumberSpotLightstring =" ";
        */
    }
    void Renderer3D::SetMeshComponentData(MeshComponent& meshComponent) {
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

        auto Transform = meshComponent.GetOwner().GetComponent<TransformComponent>();
        ModelMatrix = glm::translate(glm::mat4(1.0f),{Transform->Location + meshComponent.MeshLocalTransform.Location}) *
            glm::rotate(glm::mat4(1.0f),glm::radians(Transform->Rotation.X + meshComponent.MeshLocalTransform.Rotation.X),{1,0,0})
            * glm::rotate(glm::mat4(1.0f),glm::radians(Transform->Rotation.Y + meshComponent.MeshLocalTransform.Rotation.Y),{0,1,0})
            * glm::rotate(glm::mat4(1.0f),glm::radians(Transform->Rotation.Z + meshComponent.MeshLocalTransform.Rotation.Z),{0,0,1})
            * glm::scale(glm::mat4(1.0f),{Transform->Scale + meshComponent.MeshLocalTransform.Scale});
    
    }
    void Renderer3D::LightingErrorChecks() {
        Renderer3DInstance->m_Shader->UseShader();
        if (NumberDirectionalLight == 0) {
            Renderer3DInstance->m_Shader->SetVec3("v_DirectionalLight[0].direction",0,0,0);
            Renderer3DInstance->m_Shader->SetVec3("v_DirectionalLight[0].ambient",0,0,0);
            Renderer3DInstance->m_Shader->SetVec3("v_DirectionalLight[0].diffuse",0,0,0);
            Renderer3DInstance->m_Shader->SetVec3("v_DirectionalLight[0].specular",0,0,0);
            Renderer3DInstance->m_Shader->SetInt("v_NrDirectionalLight",1);
        }

        if (NumberPointLight == 0) {
            NumberPointLightstring = "v_PointLight[" + std::to_string(NumberPointLight) + "]";
            Renderer3DInstance->m_Shader->SetVec3(NumberPointLightstring + ".direction",0,0,0);
            Renderer3DInstance->m_Shader->SetVec3(NumberPointLightstring + ".ambient",0,0,0);
            Renderer3DInstance->m_Shader->SetVec3(NumberPointLightstring + ".diffuse",0,0,0);
            Renderer3DInstance->m_Shader->SetVec3(NumberPointLightstring + ".specular",0,0,0);
            Renderer3DInstance->m_Shader->SetFloat(NumberPointLightstring + ".constant",0);
            Renderer3DInstance->m_Shader->SetFloat(NumberPointLightstring + ".linear",0);
            Renderer3DInstance->m_Shader->SetFloat(NumberPointLightstring + ".quadratic",0);
            Renderer3DInstance->m_Shader->SetInt("v_NrPointLight",0);
        }

        if (NumberSpotLight == 0) {
            NumberSpotLightstring = "v_SpotLight[" + std::to_string(NumberSpotLight) + "]";
            Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".direction",0,0,0);
            Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".ambient",0,0,0);
            Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".diffuse",0,0,0);
            Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".specular",0,0,0);
            Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".constant",0);
            Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".linear",0);
            Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".quadratic",0);
            Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".cutOff",0);
            Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".outerCutOff",0);
            Renderer3DInstance->m_Shader->SetInt("v_NrSpotLight",0);
        }
    }
}