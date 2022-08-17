#include "Proofprch.h"
#include "World.h"
#include "Component.h"
#include "Entity.h"
#include "Script.h"
#include "Proof/Renderer/Renderer.h"
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
#include "Proof/Scene/Component.h"
#include "entt/entt.hpp"
#include "Proof/Scripting/ScriptEngine.h"
#include "Physics/PhysicsEngine.h"
namespace Proof{
	unsigned int quadVAO = 0;
	unsigned int quadVBO=0;
	
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
		m_EditorCamera.m_FarPlane = 2000;
		m_EditorCamera.m_Sensitivity = 25;
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan)return;

		CreateIBlTexture("Assets/Textures/hdr/AmbienceExposure4k.hdr");
	}
	bool World::HasEnitty(UUID ID) {
		auto it = std::find(m_Registry.entities.begin(), m_Registry.entities.end(), ID.Get());
		if (it == m_Registry.entities.end())
			return false;
		return true;
	}
	void World::OnUpdateEditor(FrameTime DeltaTime,uint32_t width,uint32_t height,bool usePBR) {
		PF_PROFILE_FUNC();
		OnUpdate(DeltaTime,width,height,usePBR);
	}

	void World::OnUpdateEditorNoDraw(FrameTime DeltaTime,uint32_t width,uint32_t height) {
		m_EditorCamera.m_FarPlane = 2000;
		m_EditorCamera.m_Sensitivity = 25;
		m_EditorCamera.OnUpdate(DeltaTime,width,height);
	}

	void World::OnUpdateRuntime(FrameTime DeltaTime, uint32_t width, uint32_t height) {
		PF_PROFILE_FUNC();
		/*
		// Scripts
		{
			auto& scriptView = m_Registry.view<NativeScriptComponent>();
			for (auto entity : scriptView) {
				auto& script = scriptView.get<NativeScriptComponent>(entity);
				if (script.Instance == nullptr)
				{

					script.Instance = script.InstantiateScript();
					script.Instance->m_Owner = Entity{ entity, this };
					script.Instance->m_World = this;
					script.Instance->OnCreate();
					script.Instance->OnSpawn();
				}
				if(script.Instance->b_CallPerframe == true)
					script.Instance->OnUpdate(DeltaTime);
			}
		}
		*/
		{
			auto& scriptView = m_Registry.view<ScriptComponent>();
			for (auto entity : scriptView) {
				auto& script = scriptView.get<ScriptComponent>(entity);
				ScriptEngine::OnUpdate(DeltaTime, Entity{ entity,this });
			}
		}
		m_PhysicsEngine->Simulate(DeltaTime);
		m_EditorCamera.OnUpdate(DeltaTime, width, height);
		{
			auto& cameraView = m_Registry.view<CameraComponent>();
			for (auto entity : cameraView) {
				auto& camera = cameraView.get<CameraComponent>(entity);
				m_SceneCamera = &camera;
				break;
			}
		}
	}
	template<typename Component>
	static Component* CopyComponentIfExists(Entity dst, Entity src)
	{
		if (src.HasComponent<Component>())
			return dst.AddorReplaceComponent<Component>(*src.GetComponent<Component>());
		return nullptr;
	}
	void World::OnSimulatePhysics(FrameTime DeltaTime,uint32_t width,uint32_t height) {
		OnUpdate(DeltaTime,width,height);
	}

	Entity World::CreateEntity(const std::string& EntName) {
		return CreateEntity(EntName, UUID());
	}

	Entity World::CreateEntity(const std::string& EntName,UUID ID) {
		/* we have to do some custmization of entt because when we pass an ID the entities create a vecot of the size of ID*/
		Entity entity = { ID,this };

		m_Registry.entities.emplace_back(ID.Get()); // not the correct way but it works there is some bugs with ent so we have to do this
		entity.AddComponent<IDComponent>(ID);
		entity.AddComponent<TagComponent>()->Tag = EntName;
		entity.AddComponent<ChildComponent>()->m_CurrentID = ID;
		entity.AddComponent<TransformComponent>()->entID = ID;
		entity.GetComponent<TransformComponent>()->m_World = this;
		return entity;
	}
	Entity World::CreateEntity(Entity entity, bool includeChildren){
		Entity newEntity = CreateEntity(entity.GetName());
		CopyComponentIfExists<TagComponent>(newEntity, entity);
		CopyComponentIfExists<TransformComponent>(newEntity, entity);
		//CopyComponentIfExists<ChildComponent>(Entity{ ID,newWorld.get() }, Entity{ ID,other.get() });
		CopyComponentIfExists<NativeScriptComponent>(newEntity, entity);
		CopyComponentIfExists<MeshComponent>(newEntity, entity);
		CopyComponentIfExists<LightComponent>(newEntity, entity);
		CopyComponentIfExists<CameraComponent>(newEntity, entity);

		CopyComponentIfExists<CubeColliderComponent>(newEntity, entity);
		CopyComponentIfExists<SphereColliderComponent>(newEntity, entity);
		CopyComponentIfExists<CapsuleColliderComponent>(newEntity, entity);
		CopyComponentIfExists<MeshColliderComponent>(newEntity, entity);
		CopyComponentIfExists<RigidBodyComponent>(newEntity, entity);

		CopyComponentIfExists<ScriptComponent>(newEntity, entity);
		if (includeChildren == true) {
			entity.EachChild([&](Entity childEntity){
				Entity newChild = CreateEntity(childEntity, true);
				newEntity.AddChild(newChild);
			});
		}
		return newEntity;
	}

	template<typename Component>
	static void CopyComponentWorld(entt::registry64& dst, entt::registry64& src, const std::unordered_map<UUID, uint64_t>& enttMap)
	{
		auto view = src.view<Component>();
		for (auto e : view)
		{
			UUID uuid = src.get<IDComponent>(e).GetID();
			uint64_t dstEnttID = enttMap.at(uuid);

			auto& component = src.get<Component>(e);
			dst.emplace_or_replace<Component>(dstEnttID, component);
		}
	}
	//static void CopyComponent
	Count<World> World::Copy(Count<World> other) {
		Count<World> newWorld = CreateCount<World>();
		newWorld->Name = other->Name;
		newWorld->m_Path = other->m_Path;
		newWorld->Name = other->Name;
		newWorld->m_EditorCamera = other->m_EditorCamera;

		newWorld->m_LastFrameHeight = other->m_LastFrameHeight;
		newWorld->m_LastFrameWidth = other->m_LastFrameWidth;

		newWorld->m_WorldCubeMap = other->m_WorldCubeMap;
		newWorld->m_WorldCubeMapIrradiance = other->m_WorldCubeMapIrradiance;
		newWorld->PrefelterMap = other->PrefelterMap;

		newWorld->m_WorldIBLTexture = other->m_WorldIBLTexture;
		newWorld->m_IBLSkyBoxBuffer= other->m_IBLSkyBoxBuffer;
		newWorld->m_IBLSkyBoxVertexArray = other->m_IBLSkyBoxVertexArray;

		newWorld->m_CaptureFBO = other->m_CaptureFBO;
		newWorld->m_CaptureRBO = other->m_CaptureRBO;

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newWorld->m_Registry;
		std::unordered_map<UUID, uint64_t> enttMap;

		// Create entities in new scene
		auto idView = srcSceneRegistry.view<IDComponent>();
		for (auto e : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).GetID();
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newWorld->CreateEntity(name,uuid);
			enttMap.insert({ uuid,newEntity.GetID() });
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponentWorld<TagComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentWorld<TransformComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentWorld<ChildComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentWorld<NativeScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentWorld<MeshComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentWorld<LightComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentWorld<CameraComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

		CopyComponentWorld<CubeColliderComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentWorld<SphereColliderComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentWorld<CapsuleColliderComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentWorld<MeshColliderComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

		CopyComponentWorld<RigidBodyComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);
		CopyComponentWorld<ScriptComponent>(dstSceneRegistry, srcSceneRegistry, enttMap);

		return newWorld;
	}

	void World::EndRuntime() {
		ScriptEngine::EndWorld();
		delete m_PhysicsEngine;
		m_PhysicsEngine = nullptr;
		m_SceneCamera = nullptr;
	}

	void World::StartRuntime(){

		{
			auto& scriptView = m_Registry.view<NativeScriptComponent>();
			for (auto entity : scriptView) {
				auto& script = scriptView.get<NativeScriptComponent>(entity);
				if (script.Instance == nullptr)
				{

					script.Instance = script.InstantiateScript();
					script.Instance->m_Owner = Entity{ entity, this };
					script.Instance->m_World = this;
					script.Instance->OnCreate();
					script.Instance->OnPlaced();
				}
			}
			{
				ScriptEngine::StartWorld(this);
				auto view = m_Registry.view<ScriptComponent>();
				for (auto e : view) {
					Entity entity = { e, this };
					ScriptEngine::OnCreate(entity);
					//ScriptEngine::OnPlace(entity);
				}
			}
		}
		m_PhysicsEngine = new PhysicsEngine(this);
		m_PhysicsEngine->Start();
	}

	void World::DeleteEntity(Entity& ent, bool deleteChildren) {
		auto it = std::find(m_Registry.entities.begin(), m_Registry.entities.end(), ent.m_ID.Get());
		if (it == m_Registry.entities.end())
			return;
		if (deleteChildren == false) {
			ent.EachChild([&](Entity childEntity) {
				childEntity.GetComponent<ChildComponent>()->m_OwnerID = 0;
				childEntity.GetComponent<ChildComponent>()->m_OwnerPointer = nullptr;
				// we are not using remove because it would change the vector we are looping causing some problems
			});
		}
		else {
			ent.EachChild([&](Entity childEntity) {
				DeleteEntity(childEntity, true);
			});
		}
		if (ent.HasOwner())
			ent.SetOwner({}); // removing the owner
		for (auto&& pdata : m_Registry.pools) {
			pdata.pool&& pdata.pool->remove(ent.m_ID.Get(), &m_Registry);
		}
		m_Registry.entities.erase(it);
		ent.m_ID = 0;
		ent.m_EnttEntity = entt::entity(0);
		ent.CurrentWorld = nullptr;
	}

	Entity World::GetEntity(UUID id) {
		return Entity{ id,this };
	}

	Entity World::FindEntityByTag(const std::string& tag) {
		ForEachEntitiesWithSingle<TagComponent>([&](Entity& entity){
			if (entity.GetComponent<TagComponent>()->Tag == tag)
				return entity;
		});
		return { 0, nullptr };
	}

	void World::OnUpdate(FrameTime DeltaTime,uint32_t width,uint32_t height,bool usePBR){
		PF_PROFILE_FUNC();
		m_EditorCamera.OnUpdate(DeltaTime,width,height);
	}

	void World::CreateIBlTexture(const std::string& filePath) {
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan)return;

		backgroundShader = Shader::GetOrCreate("IBL_Background",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/BackgroundShader.glsl");
		equirectangularToCubemapShader = Shader::GetOrCreate("IBL_CUBEMAPSHADER",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/equirectangularToCubemapShader.glsl");
		IrradianceShader = Shader::GetOrCreate("IBL_IRRADIANCESHader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/IradianceShader.glsl");
		m_WorldIBLTexture = HDRTexture::Create(filePath);
		prefilterShader = Shader::GetOrCreate("IBL_PrefeliterShader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/prefilterShader.glsl");
		brdfShader = Shader::GetOrCreate("IBL_brdfShader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/brdfShader.glsl");
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
			m_CaptureFBO->AttachColourTexture((FrameBufferTextureType)temp,0,m_WorldCubeMap->GetID(),0);
			
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
	
}