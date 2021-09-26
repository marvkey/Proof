#include "Proofprch.h"
#include "Renderer3DPBR.h"
#include "Renderer3DCore.h"
#include "../Shader.h"
#include "../VertexArray.h"
#include "../Renderer.h"

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
#include "../UniformBuffer.h"

namespace Proof{
	Count<class Texture2D>PhysicalBasedRenderer::m_WhiteTexture;
	static uint32_t Temp2 = (sizeof(glm::vec4));
	static uint32_t Temp3 = (sizeof(glm::vec4) * 2);
	static uint32_t Temp4 = (sizeof(glm::vec4) * 3);
	static uint32_t Temp5 = (sizeof(glm::vec4) * 4);
	static PhysicalBasedRenderer* s_PBRInstance;
	static glm::mat4 ModelMatrix;
	static Material s_DefaultMaterial;
	static std::vector<uint32_t> s_DifferentID;
	uint32_t NumLights=0;
	static CameraData s_CurrentCamera;
	void Renderer3DPBR::Init() {
		s_PBRInstance = new PhysicalBasedRenderer();
		s_PBRInstance->m_Shader = Shader::Create("NewPBRSHADER",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/PhysicalBasedRenderer.glsl");
		s_PBRInstance->m_VertexBuffer = VertexBuffer::Create(PhysicalBaseRendererGuide::s_MaxMesh * sizeof(PhysicalBasedRendererVertex));// can only store that amount of transforms
		s_PBRInstance->m_WhiteTexture = Texture2D::Create(1,1);
		uint32_t WhiteTextureImage = 0xffffffff;
		s_PBRInstance->m_WhiteTexture->SetData(&WhiteTextureImage,sizeof(uint32_t));
	}
	void Renderer3DPBR::BeginContext(glm::mat4 Projection,EditorCamera3D& EditorCamera) {
		s_CurrentCamera= {Projection,EditorCamera.GetCameraView(),EditorCamera.GetCameraPosition()};
		Renderer3DCore::s_CameraBuffer->SetData(&s_CurrentCamera,sizeof(CameraData));
	}
	void Renderer3DPBR::Draw(MeshComponent& meshComponent) {
		if (s_PBRInstance->SceneHasAmountMeshes(meshComponent.GetMeshPointerID()) == true) {
			auto& Map = s_PBRInstance->m_AmountMeshes.find(meshComponent.GetMeshPointerID());
			Map->second+=1;
			auto InstanceSize = s_PBRInstance->m_MeshesEndingPositionIndexTransforms.find(meshComponent.GetMeshPointerID());
			
			auto* Transform = meshComponent.GetOwner().GetComponent<TransformComponent>();
			ModelMatrix = glm::translate(glm::mat4(1.0f),{Transform->Location + meshComponent.MeshLocalTransform.Location}) *
				glm::rotate(glm::mat4(1.0f),glm::radians(Transform->Rotation.X + meshComponent.MeshLocalTransform.Rotation.X),{1,0,0})
				* glm::rotate(glm::mat4(1.0f),glm::radians(Transform->Rotation.Y + meshComponent.MeshLocalTransform.Rotation.Y),{0,1,0})
				* glm::rotate(glm::mat4(1.0f),glm::radians(Transform->Rotation.Z + meshComponent.MeshLocalTransform.Rotation.Z),{0,0,1})
				* glm::scale(glm::mat4(1.0f),{Transform->Scale + meshComponent.MeshLocalTransform.Scale});

			PhysicalBasedRendererVertex temp(ModelMatrix,meshComponent.HasMaterial() == true ? *meshComponent.GetMaterial() : s_DefaultMaterial);
			s_PBRInstance->m_Transforms.insert(s_PBRInstance->m_Transforms.begin() + InstanceSize->second,temp);
			InstanceSize->second ++;

		}else{
			s_PBRInstance->m_AmountMeshes.insert({meshComponent.GetMeshPointerID(),1});
			s_PBRInstance->m_Meshes.insert({meshComponent.GetMeshPointerID(),meshComponent});
			s_PBRInstance->m_MeshesEndingPositionIndexTransforms.insert({meshComponent.GetMeshPointerID(),s_PBRInstance->m_Transforms.size() + 1});
			s_DifferentID.emplace_back(meshComponent.GetMeshPointerID());
			/* Used For Rendering */
			meshComponent.GetModel()->m_VertexArrayObject->AddData(5,4,sizeof(PhysicalBasedRendererVertex),(void*)offsetof(PhysicalBasedRendererVertex,m_Transform));
			meshComponent.GetModel()->m_VertexArrayObject->AddData(6,4,sizeof(PhysicalBasedRendererVertex),(void*)Temp2);
			meshComponent.GetModel()->m_VertexArrayObject->AddData(7,4,sizeof(PhysicalBasedRendererVertex),(void*)Temp3);
			meshComponent.GetModel()->m_VertexArrayObject->AddData(8,4,sizeof(PhysicalBasedRendererVertex),(void*)Temp4);
			meshComponent.GetModel()->m_VertexArrayObject->AddData(9,3,sizeof(PhysicalBasedRendererVertex),(void*)offsetof(PhysicalBasedRendererVertex,PhysicalBasedRendererVertex::m_AlbedoColour));
			meshComponent.GetModel()->m_VertexArrayObject->AddData(10,1,sizeof(PhysicalBasedRendererVertex),(void*)offsetof(PhysicalBasedRendererVertex,PhysicalBasedRendererVertex::m_Matallness));
			meshComponent.GetModel()->m_VertexArrayObject->AddData(11,1,sizeof(PhysicalBasedRendererVertex),(void*)offsetof(PhysicalBasedRendererVertex,PhysicalBasedRendererVertex::m_Roughnes));
			meshComponent.GetModel()->m_VertexArrayObject->AddData(12,1,sizeof(PhysicalBasedRendererVertex),(void*)offsetof(PhysicalBasedRendererVertex,PhysicalBasedRendererVertex::m_AO));

			meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(5,1);
			meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(6,1);
			meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(7,1);
			meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(8,1);
			meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(9,1);// Material
			meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(10,1);// Material
			meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(11,1);// MaterialMaterial
			meshComponent.GetModel()->m_VertexArrayObject->AttributeDivisor(12,1);// MaterialMaterial
			meshComponent.GetModel()->m_VertexArrayObject->UnBind();
			auto* Transform = meshComponent.GetOwner().GetComponent<TransformComponent>();
			ModelMatrix = glm::translate(glm::mat4(1.0f),{Transform->Location + meshComponent.MeshLocalTransform.Location}) *
				glm::rotate(glm::mat4(1.0f),glm::radians(Transform->Rotation.X + meshComponent.MeshLocalTransform.Rotation.X),{1,0,0})
				* glm::rotate(glm::mat4(1.0f),glm::radians(Transform->Rotation.Y + meshComponent.MeshLocalTransform.Rotation.Y),{0,1,0})
				* glm::rotate(glm::mat4(1.0f),glm::radians(Transform->Rotation.Z + meshComponent.MeshLocalTransform.Rotation.Z),{0,0,1})
				* glm::scale(glm::mat4(1.0f),{Transform->Scale + meshComponent.MeshLocalTransform.Scale});
		
			PhysicalBasedRendererVertex temp(ModelMatrix,meshComponent.HasMaterial() == true ? *meshComponent.GetMaterial() : s_DefaultMaterial);
			s_PBRInstance->m_Transforms.emplace_back(temp);
		}
	}
	void Renderer3DPBR::Draw(LightComponent& lightComponent) {
		s_PBRInstance->m_Shader->UseShader();
		std::string LightPos= "lightPositions[" + std::to_string(NumLights) + "]";
		std::string LightColour= "lightColors[" + std::to_string(NumLights) + "]";
		s_PBRInstance->m_Shader->SetVec3(LightPos,{lightComponent.m_Position + lightComponent.GetOwner().GetComponent<TransformComponent>()->Location});
		s_PBRInstance->m_Shader->SetVec3(LightColour,lightComponent.m_Ambient);
		NumLights++;
	}
	PhysicalBasedRenderer* Renderer3DPBR::GetRenderer(){
		return s_PBRInstance;
	}
	void Renderer3DPBR::EndContext() {
		if(s_DifferentID.size() ==0)return;
		uint32_t sizeOffset=0;
		s_PBRInstance->m_Shader->UseShader();
		s_PBRInstance->m_Shader->SetInt("AmountLight",NumLights);
		for (uint32_t i= 0; i< s_DifferentID.size(); i++) {
			s_PBRInstance->m_VertexBuffer->Bind();
			s_PBRInstance->m_VertexBuffer->AddData(&s_PBRInstance->m_Transforms[sizeOffset],s_PBRInstance->m_Transforms.size() * sizeof(PhysicalBasedRendererVertex));
			uint32_t TempID = s_DifferentID[i];
			auto& TempMesh = s_PBRInstance->m_Meshes.find(TempID);
			auto& TempAmountMeshes = s_PBRInstance->m_AmountMeshes.find(TempID);

			if(TempMesh->second.GetMaterial()!=nullptr){
				s_PBRInstance->m_Shader->UseShader();
				s_PBRInstance->m_Shader->SetInt("albedoMap",0);
				if(TempMesh->second.GetMaterial()->AlbedoTexture !=nullptr)
					TempMesh->second.GetMaterial()->AlbedoTexture->BindTexture(0);
				else
					s_PBRInstance->m_WhiteTexture->BindTexture(0);

				s_PBRInstance->m_Shader->SetInt("normalMap",1);
				if (TempMesh->second.GetMaterial()->NormalTexture != nullptr)
					TempMesh->second.GetMaterial()->NormalTexture->BindTexture(1);
				else
					s_PBRInstance->m_WhiteTexture->BindTexture(1);

				s_PBRInstance->m_Shader->SetInt("metallicMap",2);
				if (TempMesh->second.GetMaterial()->MetallicTexture != nullptr)
					TempMesh->second.GetMaterial()->MetallicTexture->BindTexture(2);
				else
					s_PBRInstance->m_WhiteTexture->BindTexture(2);

				s_PBRInstance->m_Shader->SetInt("roughnessMap",3);
				if (TempMesh->second.GetMaterial()->RoughnessTexture != nullptr)
					TempMesh->second.GetMaterial()->RoughnessTexture->BindTexture(3);
				else
					s_PBRInstance->m_WhiteTexture->BindTexture(3);

			}else{
				s_PBRInstance->m_Shader->UseShader();
				s_PBRInstance->m_Shader->SetInt("albedoMap",0);
				s_PBRInstance->m_Shader->SetInt("normalMap",1);
				s_PBRInstance->m_Shader->SetInt("metallicMap",2);
				s_PBRInstance->m_Shader->SetInt("roughnessMap",3);
				s_PBRInstance->m_WhiteTexture->BindTexture(0);
				s_PBRInstance->m_WhiteTexture->BindTexture(1);
				s_PBRInstance->m_WhiteTexture->BindTexture(2);
				s_PBRInstance->m_WhiteTexture->BindTexture(3);
			}
			TempMesh->second.GetModel()->m_VertexArrayObject->Bind();
			TempMesh->second.GetModel()->m_IndexBufferObject->Bind();
			RendererCommand::DrawElementIndexed(TempMesh->second.GetModel()->m_VertexArrayObject,TempAmountMeshes->second);
			sizeOffset += TempAmountMeshes->second;
		}
	}
	void Renderer3DPBR::Reset() {
		s_PBRInstance->m_AmountMeshes.clear();
		s_PBRInstance->m_Meshes.clear();
		s_PBRInstance->m_MeshesEndingPositionIndexTransforms.clear();
		s_DifferentID.clear();
		s_PBRInstance->m_Transforms.clear();
		NumLights=0;
	}
}