#include "Proofprch.h"

#include "Renderer3D.h"
#include "../Shader.h"
//#include "Platform/Window/CurrentWindow.h"
#include "../VertexArray.h"
#include "../Renderer.h"

#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Camera/OrthagraphicCamera.h"
#include "Proof/Scene/Camera/EditorCamera.h"

#include "Proof/Scene/Mesh.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Material.h"
#include "../UniformBuffer.h"
/* RIGHT NOW ONLY SUPPORT 150 directional Light*/
/* RIGHT NOW ONLY SUPPORT 150 Diffuse Light*/
/* RIGHT NOW ONLY SUPPORT 150 Specular Light*/
namespace Proof
{
    static uint32_t NumberDirectionalLight = 0;
    static uint32_t NumberPointLight = 0;
    static uint32_t NumberSpotLight = 0;

    std::string NumberDirectionalLightstring;
    std::string NumberPointLightstring;
    std::string NumberSpotLightstring;

    //static Material EmptyMaterial;
    static uint32_t Temp2 = (sizeof(glm::vec4));
    static uint32_t Temp3 = (sizeof(glm::vec4) * 2);
    static uint32_t Temp4 = (sizeof(glm::vec4) * 3);
    static uint32_t Temp5 = (sizeof(glm::vec4) * 4);
    static glm::vec3 Position;
    struct InstanceRendererVertex {
        InstanceRendererVertex(const glm::mat4& Transform, const Material& mat)
            :
            m_Transform(Transform) {
            //m_MaterialAmbient = mat.m_Ambient;
            //m_MaterialDiffuse = mat.m_Diuffuse;
            //m_MaterialSpecular = mat.m_Specular;
            //m_Matallness = mat.m_Metallness;
        }
        glm::mat4 m_Transform;
        Vector m_MaterialAmbient;
        Vector m_MaterialDiffuse;
        Vector m_MaterialSpecular;
        float m_Matallness = 1.f; // default is one
    };
    struct OldCmaerData {
        glm::mat4 m_Projection;
        glm::mat4 m_View;
        OldCmaerData(const glm::mat4& projection, const glm::mat4& view) :
            m_Projection(projection), m_View(view)
        {};
    };
    static InstancedRenderer3D* Renderer3DInstance;
    Renderer3D::Render3DStats* Renderer3DStats;
    static std::vector<glm::mat4> m_Transforms;
    static std::vector<uint64_t> s_DifferentID;
    static uint32_t DifferentMeshes = 0;
    static glm::mat4 ModelMatrix;

    static std::vector<InstanceRendererVertex> m_InstanceTransforms;
    uint32_t M_UniformBufferID;

    uint32_t Renderer3D::Render3DStats::DrawCalls = 0;
    uint32_t Renderer3D::Render3DStats::NumberOfInstances = 0;
    uint32_t Renderer3D::Render3DStats::AmountDrawn = 0;
    Count<Texture2D>InstancedRenderer3D::m_WhiteTexture;
    static Count<UniformBuffer> s_CameraBuffer;
    void Renderer3D::Init() {
        //glGenBuffers(1,&M_UniformBufferID);
        //glBindBuffer(GL_UNIFORM_BUFFER,M_UniformBufferID);
        //glBufferData(GL_UNIFORM_BUFFER,sizeof(CameraData),NULL,GL_STATIC_DRAW);
        //glBindBufferBase(GL_UNIFORM_BUFFER,0,M_UniformBufferID);
        Renderer3DInstance = new InstancedRenderer3D();
        Renderer3DStats = new Renderer3D::Render3DStats;
        //s_CameraBuffer = UniformBuffer::Create(sizeof(OldCmaerData),7);
        //Renderer3DInstance->m_Shader = Shader::GetOrCreate("InstanceMeshRenderer", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/MeshShader.glsl");
        //Renderer3DInstance->m_VertexBuffer = VertexBuffer::Create(Renderer3DStats->MaxMeshes * sizeof(InstanceRendererVertex));// can only store that amount of transforms
       // Renderer3DInstance->m_WhiteTexture = Texture2D::Create(1, 1);
        uint32_t WhiteTextureImage = 0xffffffff;
        //Renderer3DInstance->m_WhiteTexture->SetData(&WhiteTextureImage,sizeof(uint32_t));

    }
    void Renderer3D::BeginContext(const PerspectiveCamera& camera) {
        //Renderer3DInstance->m_Shader->Bind();
       // Renderer3DInstance->m_Shader->SetMat4("u_Projection",camera.GetProjectionMatrix());
        //Renderer3DInstance->m_Shader->SetMat4("u_View",camera.GetViewMatrix());
    }
    void Renderer3D::BeginContext(glm::mat4 Projection, EditorCamera& EditorCamera) {
        //Renderer3DInstance->m_Shader->SetMat4("u_Projection",Projection);
        //Renderer3DInstance->m_Shader->SetMat4("u_View",EditorCamera.m_View);
        //Position = EditorCamera.m_Positon;
        //OldCmaerData temp(Projection, EditorCamera.m_View);
       // s_CameraBuffer->SetData(&temp, sizeof(OldCmaerData));

    }
    void Renderer3D::BeginContext(const OrthagraphicCamera& camera) {
       // Renderer3DInstance->m_Shader->Bind();
       // Renderer3DInstance->m_Shader->SetMat4("u_Projection",camera.GetProjectionMatrix());
       // Renderer3DInstance->m_Shader->SetMat4("u_View",camera.GetViewMatrix());
    }
    void Renderer3D::Draw(MeshComponent& meshComponent) {
        /*
        UUID meshID = meshComponent.GetMeshAssetID();
        if (Renderer3DInstance->SceneHasAmountMeshes(meshID) == true) {
            auto& map = Renderer3DInstance->m_AmountMeshes.find(meshID);
            map->second += 1;
            Renderer3DInstance->m_AmountMeshes.find(meshID)->second++;
            auto InstanceSize = Renderer3DInstance->m_MeshesEndingPositionIndexTransforms.find(meshID);

            auto Transform = meshComponent.GetOwner().GetComponent<TransformComponent>();

            InstanceRendererVertex temp(ModelMatrix,meshComponent.HasMaterial() == true ? *meshComponent.GetMaterial() : EmptyMaterial);
            m_InstanceTransforms.insert(m_InstanceTransforms.begin() + InstanceSize->second,temp);
            InstanceSize->second += 1;
            Renderer3DStats->AmountDrawn += 1;
        }
        else {
            Renderer3DInstance->m_AmountMeshes.insert({ meshID,1});
            Renderer3DInstance->m_Meshes.insert({ meshID,meshComponent});
            Renderer3DInstance->m_MeshesEndingPositionIndexTransforms.insert({ meshID,m_InstanceTransforms.size() + 1});
            s_DifferentID.emplace_back(meshID);
            DifferentMeshes++;

            SetMeshComponentData(meshComponent);

            InstanceRendererVertex temp(ModelMatrix,meshComponent.HasMaterial() == true ? *meshComponent.GetMaterial() : EmptyMaterial);
            m_InstanceTransforms.emplace_back(temp);
            Renderer3DStats->AmountDrawn += 1;
            Renderer3DStats->NumberOfInstances += 1;
        }
        */
    }
    void Renderer3D::RenderLight(LightComponent& lightComponent) {
        /*
        if (lightComponent.m_LightType == lightComponent.Direction && NumberDirectionalLight < 150) {
            NumberDirectionalLightstring = "v_DirectionalLight[" + std::to_string(NumberDirectionalLight) + "]";
            Renderer3DInstance->m_Shader->Bind();
            Renderer3DInstance->m_Shader->SetVec3(NumberDirectionalLightstring + ".direction",lightComponent.GetOwner().GetComponent<TransformComponent>().Rotation);
            Renderer3DInstance->m_Shader->SetVec3(NumberDirectionalLightstring + ".ambient",lightComponent.m_Ambient);
            Renderer3DInstance->m_Shader->SetVec3(NumberDirectionalLightstring + ".diffuse",lightComponent.m_Diffuse);
            Renderer3DInstance->m_Shader->SetVec3(NumberDirectionalLightstring + ".specular",lightComponent.m_Specular);
            NumberDirectionalLight++;
            Renderer3DInstance->m_Shader->SetInt("v_NrDirectionalLight",NumberDirectionalLight);

            return;
        }

        if (lightComponent.m_LightType == lightComponent.Point && NumberPointLight < 150) {
            NumberPointLightstring = "v_PointLight[" + std::to_string(NumberPointLight) + "]";
            Renderer3DInstance->m_Shader->Bind();
            Renderer3DInstance->m_Shader->SetVec3(NumberPointLightstring + ".position",lightComponent.GetOwner().GetComponent<TransformComponent>().Location);
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

        if (lightComponent.m_LightType == lightComponent.Spot && NumberPointLight < 150) {
            NumberSpotLightstring = "v_SpotLight[" + std::to_string(NumberSpotLight) + "]";
            Renderer3DInstance->m_Shader->Bind();
            Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".direction",lightComponent.GetOwner().GetComponent<TransformComponent>().Rotation);
            Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".position",{lightComponent.GetOwner().GetComponent<TransformComponent>().Location});
            Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".ambient",lightComponent.m_Ambient);
            Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".diffuse",lightComponent.m_Diffuse);
            Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".specular",lightComponent.m_Specular);
            Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".constant",lightComponent.m_Constant);
            Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".linear",lightComponent.m_Linear);
            Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".quadratic",lightComponent.m_Quadratic);
            Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".cutOff",Math::RCos(lightComponent.m_CutOff));
            Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".outerCutOff",Math::RCos(lightComponent.m_OuterCutOff));
            NumberSpotLight++;
            Renderer3DInstance->m_Shader->SetInt("v_NrSpotLight",NumberSpotLight);
            return;

        }
        */
    }
    void Renderer3D::EndContext() {
        if (DifferentMeshes == 0)return;
        uint32_t SizeofOffset = 0;
       /// Renderer3DInstance->m_Shader->Bind();
        LightingErrorChecks();
     //   Renderer3DInstance->m_Shader->SetVec3("viewPos",Position);
        for (uint32_t Size = 0; Size < s_DifferentID.size(); Size++)
        {
//Renderer3DInstance->m_VertexBuffer->Bind();
            //Renderer3DInstance->m_VertexBuffer->SetData(&m_InstanceTransforms[SizeofOffset], m_InstanceTransforms.size() * sizeof(InstanceRendererVertex));
            uint32_t TempID = s_DifferentID[Size];
            auto TempMesh = Renderer3DInstance->m_Meshes.find(TempID);
            auto TempAmountMeshes = Renderer3DInstance->m_AmountMeshes.find(TempID);
            //if (TempMesh->second.GetMeshSource()->textures_loaded.size() > 0) {
            //    Renderer3DInstance->m_Shader->Bind();
            //    Renderer3DInstance->m_Shader->SetInt("texture_diffuse",0);
            //    //TempMesh->second.GetMeshSource()->textures_loaded[0]->Bind(0);
            //}
            //else {
            //    Renderer3DInstance->m_Shader->Bind();
            //    Renderer3DInstance->m_Shader->SetInt("texture_diffuse",0);
            //    //Renderer3DInstance->m_WhiteTexture->Bind(0);
            //}
            /*
            TempMesh->second.GetMesh()->m_VertexArrayObject->Bind();
            TempMesh->second.GetMesh()->m_IndexBufferObject->Bind();
            RendererCommand::DrawElementIndexed(TempMesh->second.GetModel()->m_VertexArrayObject,TempAmountMeshes->second);
            Renderer3DStats->DrawCalls++;
            SizeofOffset += TempAmountMeshes->second;
            */
        }
    }

    void Renderer3D::Reset() {
        Renderer3DInstance->m_AmountMeshes.clear();
        Renderer3DInstance->m_Meshes.clear();
        Renderer3DInstance->m_MeshesEndingPositionIndexTransforms.clear();
        m_Transforms.clear();
        s_DifferentID.clear();
        m_InstanceTransforms.clear();
        DifferentMeshes = 0;
        Renderer3DStats->DrawCalls = 0;
        Renderer3DStats->AmountDrawn = 0;
        Renderer3DStats->NumberOfInstances = 0;

        NumberDirectionalLight = 0;
        NumberPointLight = 0;
        NumberSpotLight = 0;
        /*
        NumberDirectionalLightstring = " ";
        NumberPointLightstring =" ";
        NumberSpotLightstring =" ";
        */
    }
    void Renderer3D::SetMeshComponentData(MeshComponent& meshComponent) {


    }
    void Renderer3D::LightingErrorChecks() {
        //Renderer3DInstance->m_Shader->Bind();
        //if (NumberDirectionalLight == 0) {
        //    Renderer3DInstance->m_Shader->SetVec3("v_DirectionalLight[0].direction",0,0,0);
        //    Renderer3DInstance->m_Shader->SetVec3("v_DirectionalLight[0].ambient",0,0,0);
        //    Renderer3DInstance->m_Shader->SetVec3("v_DirectionalLight[0].diffuse",0,0,0);
        //    Renderer3DInstance->m_Shader->SetVec3("v_DirectionalLight[0].specular",0,0,0);
        //    Renderer3DInstance->m_Shader->SetInt("v_NrDirectionalLight",1);
        //}
        //
        //if (NumberPointLight == 0) {
        //    NumberPointLightstring = "v_PointLight[" + std::to_string(NumberPointLight) + "]";
        //    Renderer3DInstance->m_Shader->SetVec3(NumberPointLightstring + ".direction",0,0,0);
        //    Renderer3DInstance->m_Shader->SetVec3(NumberPointLightstring + ".ambient",0,0,0);
        //    Renderer3DInstance->m_Shader->SetVec3(NumberPointLightstring + ".diffuse",0,0,0);
        //    Renderer3DInstance->m_Shader->SetVec3(NumberPointLightstring + ".specular",0,0,0);
        //    Renderer3DInstance->m_Shader->SetFloat(NumberPointLightstring + ".constant",0);
        //    Renderer3DInstance->m_Shader->SetFloat(NumberPointLightstring + ".linear",0);
        //    Renderer3DInstance->m_Shader->SetFloat(NumberPointLightstring + ".quadratic",0);
        //    Renderer3DInstance->m_Shader->SetInt("v_NrPointLight",0);
        //}
        //
        //if (NumberSpotLight == 0) {
        //    NumberSpotLightstring = "v_SpotLight[" + std::to_string(NumberSpotLight) + "]";
        //    Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".direction",0,0,0);
        //    Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".ambient",0,0,0);
        //    Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".diffuse",0,0,0);
        //    Renderer3DInstance->m_Shader->SetVec3(NumberSpotLightstring + ".specular",0,0,0);
        //    Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".constant",0);
        //    Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".linear",0);
        //    Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".quadratic",0);
        //    Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".cutOff",0);
        //    Renderer3DInstance->m_Shader->SetFloat(NumberSpotLightstring + ".outerCutOff",0);
        //    Renderer3DInstance->m_Shader->SetInt("v_NrSpotLight",0);
        //}
    //}
    }
}