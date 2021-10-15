#include "Proofprch.h"
#include "World.h"
#include "Component.h"
#include "Entity.h"
#include "Script.h"
#include "Proof/Renderer/Renderer.h"
#include "Model.h"
#include "Mesh.h"
#include "Proof/Core/FrameTime.h"
#include "Component.h"
#include "Proof/Renderer/3DRenderer/Renderer3D.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Scene/Component.h"
#include "Proof/Resources/Asset/MeshAsset.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/VertexArray.h"
#include "Proof/Renderer/FrameBuffer.h"
#include<glad/glad.h>
namespace Proof{
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	
	void renderQuad() {
		if (quadVAO == 0) {
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,1.0f,0.0f,0.0f,1.0f,
				-1.0f,-1.0f,0.0f,0.0f,0.0f,
				1.0f,1.0f,0.0f,1.0f,1.0f,
				1.0f,-1.0f,0.0f,1.0f,0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1,&quadVAO);
			glGenBuffers(1,&quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
			glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),&quadVertices,GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP,0,4);
		glBindVertexArray(0);
	}
	
	World::World()
	{
		CreateIBlTexture("Assets/Textures/hdr/Arches_E_PineTree_3k.hdr");
	}
	void World::OnUpdateEditor(FrameTime DeltaTime) {
		OnUpdate(DeltaTime);
	}

	void World::OnUpdateRuntime(FrameTime DeltaTime) {
		OnUpdate(DeltaTime);
		for (NativeScriptComponent* Scripts : Registry.NativeScripts) {
			if (Scripts->m_HasScriptAttached == false) {
				continue;
			}
			if (Scripts->Instance == nullptr) {
				Scripts->Instance = Scripts->InstantiateScript();
				Scripts->Instance->OwnerEntity = Scripts->GetOwner();
				Scripts->Instance->OnCreate();
				Scripts->Instance->OnlyOnCreate();
			}
			if (Scripts->Instance->b_CallPerframe == true)
				Scripts->Instance->OnUpdate(DeltaTime);
		}

	}

	void World::OnSimulatePhysics(FrameTime DeltaTime) {
		OnUpdate(DeltaTime);
	}

	Entity World::CreateEntity(const std::string& EntName) {
		Entity entity = {Registry.Create(),this};
		entity.AddComponent<TagComponent>()->Name =EntName;
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	Entity World::CreateEntity(const std::string& EntName,uint32_t ID) {
		Entity entity = {Registry.Create(ID),this};
		entity.AddComponent<TagComponent>()->Name = EntName;
		entity.AddComponent<TransformComponent>();
		return entity;
	}
	//static void CopyComponent
	Count<World> World::Copy(Count<World> world) {
		Count<World> newWorld = CreateCount<World>("Assets/Textures/hdr/Arches_E_PineTree_3k.hdr");
		newWorld->Name = world->Name;
		newWorld->Registry = world->Registry;
		newWorld->m_Path = world->m_Path;
		/*
		newWorld->m_WorldCubeMap = m_WorldCubeMap;
		newWorld->m_WorldCubeMapIrradiance = m_WorldCubeMapIrradiance;
		newWorld->PrefelterMap = PrefelterMap;

		newWorld->m_WorldIBLTexture = m_WorldIBLTexture;
		newWorld->m_IBLSkyBoxBuffer= m_IBLSkyBoxBuffer;
		newWorld->m_IBLSkyBoxVertexArray = m_IBLSkyBoxVertexArray;

		newWorld->m_CaptureFBO = m_CaptureFBO;
		newWorld->m_CaptureRBO = m_CaptureRBO;
		newWorld
		*/
		return newWorld;
		}

	void World::EndRuntime() {
	}

	void World::OnUpdate(FrameTime DeltaTime) {
		Projection = glm::perspective(glm::radians(45.f),(float)CurrentWindow::GetWindowWidth() / (float)CurrentWindow::GetWindowHeight(),0.1f,100.0f);
		Renderer2D::BeginContext(Projection,EditorCamera.GetCameraView());
		for (SpriteComponent* Comp : Registry.SpriteComponents) {
			Renderer2D::DrawQuad(*Comp);
		}
/*
		Renderer3D::BeginContext(Projection,EditorCamera);
		for (MeshComponent* Comp : Registry.SceneMeshComponents) {
			if (Comp->GetModel()!= nullptr){
				Renderer3D::Draw(*Comp);
			}
		}

		for(LightComponent* Comp :Registry.LightComponents){
			Renderer3D::RenderLight(*Comp);
		}
		*/
		Renderer3DPBR::BeginContext(Projection,EditorCamera);
		for (MeshComponent* Comp : Registry.SceneMeshComponents) {
			if (Comp->GetModel() != nullptr) {
				Renderer3DPBR::Draw(*Comp);
			}
		}
		for (LightComponent* Comp : Registry.LightComponents) {
			Renderer3DPBR::Draw(*Comp);
		}
		Renderer3DPBR::GetRenderer()->m_Shader->Bind();
		Renderer3DPBR::GetRenderer()->m_Shader->SetInt("irradianceMap",4);
		Renderer3DPBR::GetRenderer()->m_Shader->SetInt("prefilterMap",5);
		Renderer3DPBR::GetRenderer()->m_Shader->SetInt("brdfLUT",6);

		m_WorldCubeMap->Bind(4);
		PrefelterMap->Bind(5);

		m_brdflTexture->Bind(6);
		Renderer3DPBR::EndContext();
		Renderer3DPBR::Reset();


		RendererCommand::DepthFunc(DepthType::Equal);
		backgroundShader->Bind();
		backgroundShader->SetInt("environmentMap",0);
		m_WorldCubeMap->Bind(0);
		m_IBLSkyBoxVertexArray->Bind();
		RendererCommand::DrawArray(36);
		m_IBLSkyBoxVertexArray->UnBind();
		RendererCommand::DepthFunc(DepthType::Less);
		EditorCamera.OnUpdate(DeltaTime);
	}

	void World::CreateIBlTexture(const std::string& filePath) {
		backgroundShader = Shader::Create("IBL_Background",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/BackgroundShader.glsl");
		equirectangularToCubemapShader = Shader::Create("IBL_CUBEMAPSHADER",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/equirectangularToCubemapShader.glsl");
		IrradianceShader = Shader::Create("IBL_IRRADIANCESHader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/IradianceShader.glsl");
		m_WorldIBLTexture = HDRTexture::Create(filePath);
		prefilterShader = Shader::Create("IBL_PrefeliterShader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/prefilterShader.glsl");
		brdfShader = Shader::Create("IBL_brdfShader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/brdfShader.glsl");
		//Assets\Textures\hdr
		float skyboxVertices[] = {
		// back face
			-1.0f,-1.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f, // bottom-left
			1.0f,1.0f,-1.0f,0.0f,0.0f,-1.0f,1.0f,1.0f, // top-right
			1.0f,-1.0f,-1.0f,0.0f,0.0f,-1.0f,1.0f,0.0f, // bottom-right         
			1.0f,1.0f,-1.0f,0.0f,0.0f,-1.0f,1.0f,1.0f, // top-right
			-1.0f,-1.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,0.0f, // bottom-left
			-1.0f,1.0f,-1.0f,0.0f,0.0f,-1.0f,0.0f,1.0f, // top-left
			// front face
			-1.0f,-1.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f, // bottom-left
			1.0f,-1.0f,1.0f,0.0f,0.0f,1.0f,1.0f,0.0f, // bottom-right
			1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f, // top-right
			1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f, // top-right
			-1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,0.0f,1.0f, // top-left
			-1.0f,-1.0f,1.0f,0.0f,0.0f,1.0f,0.0f,0.0f, // bottom-left
			// left face
			-1.0f,1.0f,1.0f,-1.0f,0.0f,0.0f,1.0f,0.0f, // top-right
			-1.0f,1.0f,-1.0f,-1.0f,0.0f,0.0f,1.0f,1.0f, // top-left
			-1.0f,-1.0f,-1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f, // bottom-left
			-1.0f,-1.0f,-1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f, // bottom-left
			-1.0f,-1.0f,1.0f,-1.0f,0.0f,0.0f,0.0f,0.0f, // bottom-right
			-1.0f,1.0f,1.0f,-1.0f,0.0f,0.0f,1.0f,0.0f, // top-right
			// right face
			1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,0.0f, // top-left
			1.0f,-1.0f,-1.0f,1.0f,0.0f,0.0f,0.0f,1.0f, // bottom-right
			1.0f,1.0f,-1.0f,1.0f,0.0f,0.0f,1.0f,1.0f, // top-right         
			1.0f,-1.0f,-1.0f,1.0f,0.0f,0.0f,0.0f,1.0f, // bottom-right
			1.0f,1.0f,1.0f,1.0f,0.0f,0.0f,1.0f,0.0f, // top-left
			1.0f,-1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f, // bottom-left     
		   // bottom face
		   -1.0f,-1.0f,-1.0f,0.0f,-1.0f,0.0f,0.0f,1.0f, // top-right
		   1.0f,-1.0f,-1.0f,0.0f,-1.0f,0.0f,1.0f,1.0f, // top-left
		   1.0f,-1.0f,1.0f,0.0f,-1.0f,0.0f,1.0f,0.0f, // bottom-left
		   1.0f,-1.0f,1.0f,0.0f,-1.0f,0.0f,1.0f,0.0f, // bottom-left
		   -1.0f,-1.0f,1.0f,0.0f,-1.0f,0.0f,0.0f,0.0f, // bottom-right
		   -1.0f,-1.0f,-1.0f,0.0f,-1.0f,0.0f,0.0f,1.0f, // top-right
		   // top face
		   -1.0f,1.0f,-1.0f,0.0f,1.0f,0.0f,0.0f,1.0f, // top-left
		   1.0f,1.0f,1.0f,0.0f,1.0f,0.0f,1.0f,0.0f, // bottom-right
		   1.0f,1.0f,-1.0f,0.0f,1.0f,0.0f,1.0f,1.0f, // top-right     
		   1.0f,1.0f,1.0f,0.0f,1.0f,0.0f,1.0f,0.0f, // bottom-right
		   -1.0f,1.0f,-1.0f,0.0f,1.0f,0.0f,0.0f,1.0f, // top-left
		   -1.0f,1.0f,1.0f,0.0f,1.0f,0.0f,0.0f,0.0f  // bottom-left        
		};
		m_IBLSkyBoxBuffer = VertexBuffer::Create(&skyboxVertices,sizeof(skyboxVertices));
		m_IBLSkyBoxVertexArray = VertexArray::Create();
		m_IBLSkyBoxVertexArray->AddData(0,3,8 * sizeof(float),(void*)0);
		m_IBLSkyBoxVertexArray->AddData(1,3,8 * sizeof(float),(void*)(3 * sizeof(float)));
		m_IBLSkyBoxVertexArray->AddData(2,2,8 * sizeof(float),(void*)(6 * sizeof(float)));
		backgroundShader->Bind();
		backgroundShader->SetInt("environmentMap",0);
		m_CaptureFBO = FrameBuffer::Create();
		m_CaptureFBO->Bind();
		m_CaptureRBO = RenderBuffer::Create(RenderBufferAttachment::DepthComponent24,512,512);
		m_CaptureFBO->AttachRenderBuffer(FrameBufferAttachmentType::DepthAttachment,m_CaptureRBO->GetID());

		m_WorldCubeMap = CubeMap::Create();

		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f),1.0f,0.1f,10.0f);
		glm::mat4 captureViews[] =
		{
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,0.0f,0.0f),glm::vec3(0.0f,-1.0f,0.0f)),
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(-1.0f,0.0f,0.0f),glm::vec3(0.0f,-1.0f,0.0f)),
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f)),
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,-1.0f,0.0f),glm::vec3(0.0f,0.0f,-1.0f)),
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f),glm::vec3(0.0f,-1.0f,0.0f)),
			glm::lookAt(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,-1.0f),glm::vec3(0.0f,-1.0f,0.0f))
		};

		equirectangularToCubemapShader->Bind();
		equirectangularToCubemapShader->SetInt("equirectangularMap",0);
		equirectangularToCubemapShader->SetMat4("projection",captureProjection);
		m_WorldIBLTexture->Bind(0);
		RendererCommand::SetViewPort(512,512);
		for (uint32_t i = 0; i < 6; ++i) {
			equirectangularToCubemapShader->SetMat4("view",captureViews[i]);
			uint32_t temp = (uint32_t)FrameBufferTextureType::CubeMapPosX;
			temp += i;
			m_CaptureFBO->AttachColourTexture((FrameBufferTextureType)temp,0,m_WorldCubeMap->GetID());

			RendererCommand::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);
			RendererCommand::DepthFunc(DepthType::Equal);
			m_IBLSkyBoxVertexArray->Bind();
			RendererCommand::DrawArray(36);
			m_IBLSkyBoxVertexArray->UnBind();
			RendererCommand::DepthFunc(DepthType::Less);
		}
		m_CaptureFBO->UnBind();
		m_WorldCubeMap->GenerateMipMap();
		m_WorldCubeMapIrradiance = CubeMap::Create(32,32);
		m_CaptureFBO->Bind();
		m_CaptureRBO->Bind();
		m_CaptureRBO->Remap(32,32,RenderBufferAttachment::DepthComponent24);

		IrradianceShader->Bind();
		IrradianceShader->SetInt("environmentMap",0);
		IrradianceShader->SetMat4("projection",captureProjection);

		m_WorldCubeMap->Bind(0);
		RendererCommand::SetViewPort(32,32);
		m_CaptureFBO->Bind();

		for (unsigned int i = 0; i < 6; ++i) {
			IrradianceShader->SetMat4("view",captureViews[i]);
			uint32_t temp = (uint32_t)FrameBufferTextureType::CubeMapPosX;
			temp += i;
			m_CaptureFBO->AttachColourTexture((FrameBufferTextureType)temp,0,m_WorldCubeMapIrradiance->GetID());

			RendererCommand::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);

			RendererCommand::DepthFunc(DepthType::Equal);
			m_IBLSkyBoxVertexArray->Bind();
			RendererCommand::DrawArray(36);
			m_IBLSkyBoxVertexArray->UnBind();
			RendererCommand::DepthFunc(DepthType::Less);
		}
		m_CaptureFBO->UnBind();
		PrefelterMap = CubeMap::Create(128,128,true);
		prefilterShader->Bind();
		prefilterShader->SetInt("environmentMap",0);
		prefilterShader->SetMat4("projection",captureProjection);

		m_WorldCubeMap->Bind(0);
		m_CaptureFBO->Bind();
		unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; ++mip) {
			unsigned int mipWidth = 128 * std::pow(0.5,mip);
			unsigned int mipHeight = 128 * std::pow(0.5,mip);
			m_CaptureRBO->Bind();
			m_CaptureRBO->Remap(mipWidth,mipHeight,RenderBufferAttachment::DepthComponent24);
			RendererCommand::SetViewPort(mipWidth,mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			prefilterShader->SetFloat("roughness",roughness);
			for (unsigned int i = 0; i < 6; ++i) {
				prefilterShader->SetMat4("view",captureViews[i]);
				uint32_t temp = (uint32_t)FrameBufferTextureType::CubeMapPosX;
				temp += i;
				m_CaptureFBO->AttachColourTexture((FrameBufferTextureType)temp,0,PrefelterMap->GetID(),mip);
				RendererCommand::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);

				RendererCommand::DepthFunc(DepthType::Equal);
				m_IBLSkyBoxVertexArray->Bind();
				RendererCommand::DrawArray(36);
				m_IBLSkyBoxVertexArray->UnBind();
				RendererCommand::DepthFunc(DepthType::Less);
			}
		}

		m_brdflTexture = Texture2D::Create(512,512,DataFormat::RG,InternalFormat::R16F,TextureBaseTypes::ClampToEdge,TextureBaseTypes::ClampToEdge,TextureBaseTypes::Linear,TextureBaseTypes::Linear,type::Float);
		m_CaptureFBO->Bind();
		m_CaptureRBO->Bind();
		m_CaptureRBO->Remap(512,512,RenderBufferAttachment::DepthComponent24);
		m_CaptureFBO->AttachColourTexture(FrameBufferTextureType::Texture2D,0,m_brdflTexture->GetID());
		RendererCommand::SetViewPort(512,512);

		brdfShader->Bind();

		RendererCommand::Clear(ProofClear::ColourBuffer | ProofClear::DepthBuffer);
		RendererCommand::DepthFunc(DepthType::Equal);
		renderQuad();
		RendererCommand::DepthFunc(DepthType::Less);
		m_CaptureFBO->UnBind();
		RendererCommand::SetViewPort(CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight());
	}

	template<class T>
	void World::OnComponentAdded(Entity _Entity,T* component) {
		PF_CORE_ASSERT(false,"Component Not Identified");
	}
	template<>
	void World::OnComponentAdded(Entity _Entity,NativeScriptComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;
	}
	template<>
	void World::OnComponentAdded(Entity _Entity,TransformComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;

	}
	template<>
	void World::OnComponentAdded(Entity _Entity,TagComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;

	}
	template<>
	void World::OnComponentAdded(Entity _Entity,MeshComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;
	}

	template<>
	void World::OnComponentAdded(Entity _Entity,SpriteComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;
	}

	template<>
	void World::OnComponentAdded(Entity _Entity,LightComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;
	}
}