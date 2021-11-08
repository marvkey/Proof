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
	uint32_t NumLights=0;
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
		s_PBRInstance->m_Shader->Bind();
		std::string LightPos= "lightPositions[" + std::to_string(NumLights) + "]";
		std::string LightColour= "lightColors[" + std::to_string(NumLights) + "]";
		s_PBRInstance->m_DeferedRendering.LightShader->SetVec3(LightPos,{lightComponent.GetOwner().GetComponent<TransformComponent>()->Location});
		s_PBRInstance->m_DeferedRendering.LightShader->SetVec3(LightColour,lightComponent.m_Ambient);
		NumLights++;
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
		NumLights=0;
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
		if (s_DifferentID.size() == 0)return;
		RendererCommand::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);
		s_PBRInstance->m_DeferedRendering.LightShader->Bind();
		s_PBRInstance->m_DeferedRendering.GPosition->Bind(0);
		s_PBRInstance->m_DeferedRendering.GNormal->Bind(1);
		s_PBRInstance->m_DeferedRendering.GAlbedo->Bind(2);
		renderQuad11();
	}
	void Renderer3DPBR::Render() {
		RenderMesh();
		RenderLight();
		if (s_DifferentID.size() >0)
			s_PBRInstance->m_DeferedRendering.Gbuffer->WriteBuffer(Application::GetScreenBuffer()->GetFrameBufferID());
	//	glBlitFramebuffer(0,0,CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight(),0,0,CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight(),GL_DEPTH_BUFFER_BIT,GL_NEAREST);
		//glBindFramebuffer(GL_FRAMEBUFFER,(Application::GetScreenBuffer()->GetFrameBufferID()));
	}
	void Renderer3DPBR::RenderMesh() {
		if (s_DifferentID.size() == 0)return;
		RendererCommand::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);
		RendererCommand::SetClearColor(0.1f,0.1f,0.1f,1.0f);
		s_PBRInstance->m_DeferedRendering.Gbuffer->Bind();
		uint32_t sizeOffset = 0;
		for (uint32_t i = 0; i < s_DifferentID.size(); i++) {
			uint32_t TempID = s_DifferentID[i];
			auto& TempMesh = s_PBRInstance->m_Meshes.find(TempID);
			auto& TempAmountMeshes = s_PBRInstance->m_AmountMeshes.find(TempID);
			s_PBRInstance->m_DeferedRendering.MeshShader->Bind();
			s_PBRInstance->m_VertexBuffer->Bind();
			s_PBRInstance->m_VertexBuffer->AddData(&s_PBRInstance->m_Transforms[sizeOffset],TempAmountMeshes->second * sizeof(PhysicalBasedRendererVertex));
			if (TempMesh->second.GetMaterial() != nullptr) {
				s_PBRInstance->m_DeferedRendering.MeshShader->Bind();
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
				s_PBRInstance->m_Shader->Bind();
				s_PBRInstance->m_Shader->SetInt("albedoMap",0);
				s_PBRInstance->m_Shader->SetInt("normalMap",1);
				s_PBRInstance->m_Shader->SetInt("metallicMap",2);
				s_PBRInstance->m_Shader->SetInt("roughnessMap",3);
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
			s_PBRInstance->m_Shader->UnBind();
		}
		s_PBRInstance->m_DeferedRendering.Gbuffer->UnBind();
	}
	DeferedRendering::DeferedRendering() {
		MeshShader = Shader::Create("MeshShader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/Proof/deferedShading/MeshGeometry.glsl");
		LightShader = Shader::Create("LightShader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/Proof/deferedShading/LighteningPass.glsl");
		Gbuffer = FrameBuffer::Create();
		GPosition = Texture2D::Create(800,600,DataFormat::RGBA,InternalFormat::RGBA16F,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,type::Float,false);
		Gbuffer->AttachColourTexture(FrameBufferTextureType::Texture2D,0,GPosition->GetID());

		GNormal = Texture2D::Create(800,600,DataFormat::RGBA,InternalFormat::RGBA16F,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,type::Float,false);
		Gbuffer->AttachColourTexture(FrameBufferTextureType::Texture2D,1,GNormal->GetID());

		GAlbedo = Texture2D::Create(800,600,DataFormat::RGBA,InternalFormat::RGBA16F,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,TextureBaseTypes::Nearest,type::UnsignedByte,false);
		Gbuffer->AttachColourTexture(FrameBufferTextureType::Texture2D,2,GAlbedo->GetID());
		unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2};
		glDrawBuffers(3,attachments);
		RenderBuffer = RenderBuffer::Create(RenderBufferAttachment::DepthComponent,800,600);
		Gbuffer->AttachRenderBuffer(FrameBufferAttachmentType::DepthAttachment,RenderBuffer->GetID());
	}
}