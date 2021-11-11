#include "Proofprch.h"
#include "Renderer3DPBR.h"
#include "Renderer3DCore.h"
#include "../Shader.h"
#include "../VertexArray.h"
#include "../Renderer.h"

#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Camera/OrthagraphicCamera.h"
#include "Proof/Scene/Camera/EditorCamera.h"

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
	uint32_t NumDirLights=0;
	uint32_t NumPointLights=0;
	uint32_t NumSpotLights=0;
	static CameraData s_CurrentCamera;
	static DrawType s_WorldDrawType = DrawType::Triangles;
	void Renderer3DPBR::Init() {
		s_PBRInstance = new PhysicalBasedRenderer();
		s_PBRInstance->m_Shader = Shader::Create("NewPBRSHADER",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/PhysicalBasedRenderer.glsl");
		s_PBRInstance->m_VertexBuffer = VertexBuffer::Create(PhysicalBaseRendererGuide::s_MaxMesh * sizeof(PhysicalBasedRendererVertex));// can only store that amount of transforms
		s_PBRInstance->m_WhiteTexture = Texture2D::Create(1,1);
		uint32_t WhiteTextureImage = 0xffffffff;
		s_PBRInstance->m_WhiteTexture->SetData(&WhiteTextureImage,sizeof(uint32_t));
	}
	void Renderer3DPBR::BeginContext(EditorCamera& editorCamera) {
		s_CurrentCamera= {editorCamera.m_Projection,editorCamera.m_View,editorCamera.m_Positon};
		Renderer3DCore::s_CameraBuffer->SetData(&s_CurrentCamera,sizeof(CameraData));
	}
	void Renderer3DPBR::BeginContext(const glm::mat4& projection,const glm::mat4& view,const Vector& Position) {
		s_CurrentCamera = {projection,view,Position};
		Renderer3DCore::s_CameraBuffer->SetData(&s_CurrentCamera,sizeof(CameraData));
	}
	void Renderer3DPBR::Draw(MeshComponent& meshComponent) {
		int usingMaterial = meshComponent.HasMaterial();

		if (s_PBRInstance->SceneHasAmountMeshes(meshComponent.GetMeshPointerID()) == true) {
			auto& Map = s_PBRInstance->m_AmountMeshes.find(meshComponent.GetMeshPointerID());
			Map->second+=1;
			auto InstanceSize = s_PBRInstance->m_MeshesEndingPositionIndexTransforms.find(meshComponent.GetMeshPointerID());
			
			auto* Transform = meshComponent.GetOwner().GetComponent<TransformComponent>();
			ModelMatrix = Transform->GetWorldTransform();
			PhysicalBasedRendererVertex temp(ModelMatrix,meshComponent.HasMaterial() == true ? *meshComponent.GetMaterial() : s_DefaultMaterial,usingMaterial);
			s_PBRInstance->m_Transforms.insert(s_PBRInstance->m_Transforms.begin() + InstanceSize->second,temp);
			InstanceSize->second ++;

		}else{

			s_PBRInstance->m_AmountMeshes.insert({meshComponent.GetMeshPointerID(),1});
			s_PBRInstance->m_Meshes.insert({meshComponent.GetMeshPointerID(),meshComponent});
			s_PBRInstance->m_MeshesEndingPositionIndexTransforms.insert({meshComponent.GetMeshPointerID(),s_PBRInstance->m_Transforms.size() + 1});
			s_DifferentID.emplace_back(meshComponent.GetMeshPointerID());
			auto* Transform = meshComponent.GetOwner().GetComponent<TransformComponent>();
			ModelMatrix = Transform->GetWorldTransform();
		
			PhysicalBasedRendererVertex temp(ModelMatrix,meshComponent.HasMaterial() == true ? *meshComponent.GetMaterial() : s_DefaultMaterial,usingMaterial);
			s_PBRInstance->m_Transforms.emplace_back(temp);
		}
	}
	void Renderer3DPBR::Draw(LightComponent& lightComponent) {
		s_PBRInstance->m_DeferedRendering.LightShader->Bind();
	
		if (lightComponent.m_LightType == lightComponent.Direction && NumDirLights < 150) {
			std::string mumberDirectionalLightstring = "v_DirectionalLight[" + std::to_string(NumDirLights) + "]";
			s_PBRInstance->m_DeferedRendering.LightShader->Bind();
			s_PBRInstance->m_DeferedRendering.LightShader->SetVec3(mumberDirectionalLightstring + ".Direction",lightComponent.GetOwner().GetComponent<TransformComponent>()->Rotation);
			s_PBRInstance->m_DeferedRendering.LightShader->SetVec3(mumberDirectionalLightstring + ".Ambient",lightComponent.m_Ambient);
			NumDirLights++;
			return;
		}

		if (lightComponent.m_LightType == lightComponent.Point && NumPointLights < 150) {
			std::string numberPointLightstring = "v_PointLight[" + std::to_string(NumPointLights) + "]";
			s_PBRInstance->m_DeferedRendering.LightShader->SetVec3(numberPointLightstring + ".Position",lightComponent.GetOwner().GetComponent<TransformComponent>()->Location);
			s_PBRInstance->m_DeferedRendering.LightShader->SetVec3(numberPointLightstring + ".Ambient",lightComponent.m_Ambient);
			s_PBRInstance->m_DeferedRendering.LightShader->SetFloat(numberPointLightstring + + ".Constant",lightComponent.m_Constant);
			s_PBRInstance->m_DeferedRendering.LightShader->SetFloat(numberPointLightstring + + ".Linear",lightComponent.m_Linear);
			s_PBRInstance->m_DeferedRendering.LightShader->SetFloat(numberPointLightstring + + ".Quadratic",lightComponent.m_Quadratic);
			s_PBRInstance->m_DeferedRendering.LightShader->SetFloat(numberPointLightstring + + ".Radius",lightComponent.Radius);
			NumPointLights++;
			return;
		}

		if (lightComponent.m_LightType == lightComponent.Spot && NumSpotLights< 150) {
			std::string numberSpotLightstring = "v_SpotLight[" + std::to_string(NumSpotLights) + "]";
			s_PBRInstance->m_DeferedRendering.LightShader->SetVec3(numberSpotLightstring + ".Direction",lightComponent.GetOwner().GetComponent<TransformComponent>()->Rotation);
			s_PBRInstance->m_DeferedRendering.LightShader->SetVec3(numberSpotLightstring + ".Position",{lightComponent.GetOwner().GetComponent<TransformComponent>()->Location});
			s_PBRInstance->m_DeferedRendering.LightShader->SetVec3(numberSpotLightstring + ".Ambient",lightComponent.m_Ambient);
			s_PBRInstance->m_DeferedRendering.LightShader->SetFloat(numberSpotLightstring  +".Constant",lightComponent.m_Constant);
			s_PBRInstance->m_DeferedRendering.LightShader->SetFloat(numberSpotLightstring  +".Linear",lightComponent.m_Linear);
			s_PBRInstance->m_DeferedRendering.LightShader->SetFloat(numberSpotLightstring  +".Quadratic",lightComponent.m_Quadratic);
			s_PBRInstance->m_DeferedRendering.LightShader->SetFloat(numberSpotLightstring  +".Radius",lightComponent.Radius);
			s_PBRInstance->m_DeferedRendering.LightShader->SetFloat(numberSpotLightstring + ".CutOff",Math::Cos(Math::Radian(lightComponent.m_CutOff)));
			s_PBRInstance->m_DeferedRendering.LightShader->SetFloat(numberSpotLightstring + ".OuterCutOff",Math::Cos(Math::Radian(lightComponent.m_OuterCutOff)));
			NumSpotLights++;
			return;

		}
	}
	PhysicalBasedRenderer* Renderer3DPBR::GetRenderer(){
		return s_PBRInstance;
	}
	void Renderer3DPBR::EndContext() {
		Render();
		Reset();
	}
	void Renderer3DPBR::Reset() {
		s_PBRInstance->m_AmountMeshes.clear();
		s_PBRInstance->m_Meshes.clear();
		s_PBRInstance->m_MeshesEndingPositionIndexTransforms.clear();
		s_DifferentID.clear();
		s_PBRInstance->m_Transforms.clear();
		NumDirLights = 0;
		NumPointLights = 0;
		NumSpotLights = 0;
	}
	unsigned int quadVAO1 = 0;
	unsigned int quadVBO1;
	void renderQuad11() {
		if (quadVAO1 == 0) {
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,1.0f,0.0f,0.0f,1.0f,
				-1.0f,-1.0f,0.0f,0.0f,0.0f,
				1.0f,1.0f,0.0f,1.0f,1.0f,
				1.0f,-1.0f,0.0f,1.0f,0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1,&quadVAO1);
			glGenBuffers(1,&quadVBO1);
			glBindVertexArray(quadVAO1);
			glBindBuffer(GL_ARRAY_BUFFER,quadVBO1);
			glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),&quadVertices,GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO1);
		glDrawArrays(GL_TRIANGLE_STRIP,0,4);
		glBindVertexArray(0);
	}
	void Renderer3DPBR::RenderLight() {
		Application::GetScreenBuffer()->Bind();
		RendererCommand::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);
		s_PBRInstance->m_DeferedRendering.LightShader->Bind();
		
		s_PBRInstance->m_DeferedRendering.LightShader->SetInt("gPosition",0);
		s_PBRInstance->m_DeferedRendering.LightShader->SetInt("gNormal",1);
		s_PBRInstance->m_DeferedRendering.LightShader->SetInt("gAlbedoSpec",2);
		s_PBRInstance->m_DeferedRendering.GPosition->Bind(0);
		s_PBRInstance->m_DeferedRendering.GNormal->Bind(1);
		s_PBRInstance->m_DeferedRendering.GAlbedo->Bind(2);
		s_PBRInstance->m_DeferedRendering.LightShader->SetInt("v_NrDirectionalLight",NumDirLights);
		s_PBRInstance->m_DeferedRendering.LightShader->SetInt("v_NrPointLight",NumPointLights);
		s_PBRInstance->m_DeferedRendering.LightShader->SetInt("v_NrSpotLight",NumSpotLights);
		renderQuad11();
		Application::GetScreenBuffer()->UnBind();
	}
	void Renderer3DPBR::Render() {
		RenderMesh();
		RenderLight();
		if(s_DifferentID.size()==0)
			return;
		s_PBRInstance->m_DeferedRendering.Gbuffer->WriteBuffer(Application::GetScreenBuffer()->GetFrameBufferID());
		Application::GetScreenBuffer()->Bind();
		glBlitFramebuffer(0,0,CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight(),0,0,CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight(),GL_DEPTH_BUFFER_BIT,GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER,(Application::GetScreenBuffer()->GetFrameBufferID()));
	}
	void Renderer3DPBR::RenderMesh() {
		if (s_DifferentID.size() == 0)return;
		RendererCommand::SetClearColor(0.1f,0.1f,0.1f,1.0f);
		RendererCommand::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);
		s_PBRInstance->m_DeferedRendering.Gbuffer->Bind();
		RendererCommand::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);
		uint32_t sizeOffset = 0;
		for (uint32_t i = 0; i < s_DifferentID.size(); i++) {
			uint32_t TempID = s_DifferentID[i];
			auto& TempMesh = s_PBRInstance->m_Meshes.find(TempID);
			auto& TempAmountMeshes = s_PBRInstance->m_AmountMeshes.find(TempID);
			s_PBRInstance->m_DeferedRendering.MeshShader->Bind();
			s_PBRInstance->m_VertexBuffer->Bind();
			s_PBRInstance->m_VertexBuffer->AddData(&s_PBRInstance->m_Transforms[sizeOffset],TempAmountMeshes->second * sizeof(PhysicalBasedRendererVertex));
			s_PBRInstance->m_DeferedRendering.MeshShader->Bind();

			if (TempMesh->second.GetMaterial() != nullptr) {
				
				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("albedoMap",0);
				if (TempMesh->second.GetMaterial()->AlbedoTexture != nullptr)
					TempMesh->second.GetMaterial()->AlbedoTexture->Bind(0);
				else
					s_PBRInstance->m_WhiteTexture->Bind(0);

				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("normalMap",1);
				if (TempMesh->second.GetMaterial()->NormalTexture != nullptr)
					TempMesh->second.GetMaterial()->NormalTexture->Bind(1);
				else
					s_PBRInstance->m_WhiteTexture->Bind(1);

				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("metallicMap",2);
				if (TempMesh->second.GetMaterial()->MetallicTexture != nullptr)
					TempMesh->second.GetMaterial()->MetallicTexture->Bind(2);
				else
					s_PBRInstance->m_WhiteTexture->Bind(2);

				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("roughnessMap",3);
				if (TempMesh->second.GetMaterial()->RoughnessTexture != nullptr)
					TempMesh->second.GetMaterial()->RoughnessTexture->Bind(3);
				else
					s_PBRInstance->m_WhiteTexture->Bind(3);
			}
			else {
				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("albedoMap",0);
				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("normalMap",1);
				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("metallicMap",2);
				s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("roughnessMap",3);
				s_PBRInstance->m_WhiteTexture->Bind(0);
				s_PBRInstance->m_WhiteTexture->Bind(1);
				s_PBRInstance->m_WhiteTexture->Bind(2);
				s_PBRInstance->m_WhiteTexture->Bind(3);

			}
			
			
			if (TempMesh->second.GetMesh()->m_FaceCulling == true)
				RendererCommand::Enable(ProofRenderTest::CullFace);
			if (TempMesh->second.GetMesh()->m_Enabled == true) {
				for (SubMesh& mesh : TempMesh->second.GetMesh()->meshes) {
					if (mesh.m_Enabled == false)
						continue;
					
					if (TempMesh->second.HasMaterial() == false) {
						s_PBRInstance->m_DeferedRendering.MeshShader->SetInt("DiffuseShader",7);
						if (mesh.m_Textures.size() > 0)
							mesh.m_Textures[0]->Bind(7);
						else
							s_PBRInstance->m_WhiteTexture->Bind(7);
					}
					
					mesh.m_VertexArrayObject->Bind();
					mesh.m_IndexBufferObject->Bind();
					s_WorldDrawType = DrawType::Triangles;
					RendererCommand::DrawElementIndexed(mesh.m_VertexArrayObject,TempAmountMeshes->second,s_WorldDrawType);
				}
			}
			RendererCommand::Disable(ProofRenderTest::CullFace); // rename to render settings
			sizeOffset += TempAmountMeshes->second;
			s_PBRInstance->m_DeferedRendering.MeshShader->UnBind();
		}
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}
	void Renderer3DPBR::LightErrorChecks() {
		s_PBRInstance->m_DeferedRendering.LightShader->Bind();
		/*
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
		*/
	}
	DeferedRendering::DeferedRendering() {
		MeshShader = Shader::Create("MeshShader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/Proof/deferedShading/MeshGeometry.glsl");
		LightShader = Shader::Create("LightShader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/Proof/deferedShading/LighteningPass.glsl");
		Gbuffer = FrameBuffer::Create();
		Gbuffer->Bind();
		GPosition = Texture2D::Create(CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight(),DataFormat::RGBA,InternalFormat::RGBA16F,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,type::Float,false);
		Gbuffer->AttachColourTexture(FrameBufferTextureType::Texture2D,0,GPosition->GetID());

		GNormal = Texture2D::Create(CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight(),DataFormat::RGBA,InternalFormat::RGBA16F,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,type::Float,false);
		Gbuffer->AttachColourTexture(FrameBufferTextureType::Texture2D,1,GNormal->GetID());

		GAlbedo = Texture2D::Create(CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight(),DataFormat::RGBA,InternalFormat::RGBA,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,type::UnsignedByte,false);
		Gbuffer->AttachColourTexture(FrameBufferTextureType::Texture2D,2,GAlbedo->GetID());
		unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2};
		glDrawBuffers(3,attachments);
		RenderBuffer = RenderBuffer::Create(RenderBufferAttachment::DepthComponent,CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight());
		Gbuffer->AttachRenderBuffer(FrameBufferAttachmentType::DepthAttachment,RenderBuffer->GetID());
		Gbuffer->UnBind();
	}
}