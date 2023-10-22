#include "Proofprch.h"
#include "World.h"
#include "Component.h"
#include "Entity.h"
#include "Script.h"
#include "Mesh.h"
#include "Proof/Core/FrameTime.h"
#include "Component.h"
#include "Proof/Scene/Component.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Scene/Component.h"
#include "entt/entt.hpp"
#include "Proof/Scripting/ScriptEngine.h"
#include "Proof/Physics/PhysicsEngine.h"
#include "Proof/Physics/PhysicsWorld.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Input/InputManager.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Scene/Prefab.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Core/Application.h"
#include "Proof/Audio/AudioEngine.h"
#include "Proof/Audio/AudioUtils.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Physics/PhysicsActor.h"
#include "Proof/Physics/PhysicsMeshCache.h"
#include "Proof/Physics/MeshCollider.h"

#include "Proof/Scripting/ScriptWorld.h"
namespace Proof {
	World::World(const std::string& name, UUID ID):
		Name(name)
	{
		m_ScriptWorld = Count<ScriptWorld>::Create(this);
		Init();

		//m_Registry.on_destroy<ChildComponent>().connect<&World::OnChildComponentDestroy>(this);

	}
	World::~World()
	{
		m_Registry.clear();
	}
	bool World::HasEntity(UUID ID)const {
		if (ID == 0)
			return false;
		return m_EntitiesMap.contains(ID);
	}
	
	void World::OnUpdateEditor(FrameTime DeltaTime) {
		PF_PROFILE_FUNC();
		DeleteEntitiesfromQeue();
		{
			PF_PROFILE_FUNC("World::OnUpdateEditor - Particle OnUpdate");

			const auto& particleView = m_Registry.view<ParticleSystemComponent>();
			for (auto entity : particleView)
			{
				auto& particleSystem = particleView.get<ParticleSystemComponent>(entity);
				if (particleSystem.ParticleHandlerTable == nullptr)continue;
				Entity wfadfas{ entity,this };
				for (auto& [index, handler] : particleSystem.ParticleHandlerTable->GetHandlers())
				{
					if (handler != nullptr)
					{
						if(handler->Visible == true)
							handler->Update(DeltaTime,GlmVecToProof( GetWorldSpaceLocation(wfadfas)));
					}
				}
			}
		}
	}
	

	void World::OnRender(Count<class WorldRenderer> worldRenderer, FrameTime timestep, const Camera& camera, const Vector& cameraLocation, float nearPlane, float farPlane)
	{
		PF_PROFILE_FUNC();
		m_Camera = camera;
		#if 1
		worldRenderer->SetContext(this);
		worldRenderer->BeginScene(camera, cameraLocation, nearPlane, farPlane);

		// lighting
		{
			//directional lights
			{
				auto dirLights = m_Registry.group<DirectionalLightComponent>(entt::get<TransformComponent>);
				SBDirectionalLightsSceneData directionaLightScene;
				directionaLightScene.DirectionalLights.resize(dirLights.size());
				int index = 0;
				for (auto& entityID : dirLights)
				{
					Entity entity(entityID, this);
					const auto& dirLightComponent = dirLights.get<DirectionalLightComponent>(entityID);
					glm::vec3 direction =GetWorldSpaceRotation(entity);
					direction = glm::normalize(direction);
					directionaLightScene.DirectionalLights[index].Color = dirLightComponent.Color;
					directionaLightScene.DirectionalLights[index].Intensity = dirLightComponent.Intensity;
					directionaLightScene.DirectionalLights[index].Direction = -direction;
					directionaLightScene.DirectionalLights[index].ShadowSoftness = dirLightComponent.ShadowSoftness;
					directionaLightScene.DirectionalLights[index].ShadowStrength = dirLightComponent.ShadowStrength;
					directionaLightScene.DirectionalLights[index].bCastShadows = dirLightComponent.CastShadow;
					directionaLightScene.DirectionalLights[index].bCastSoftShadows = dirLightComponent.CastSoftShadow;

					index++;
				}
				if(!dirLights.empty())
					worldRenderer->SubmitDirectionalLight(directionaLightScene);
			}

			// sky Light
			{
				auto skylights = m_Registry.group<SkyLightComponent>(entt::get<TransformComponent>);
				if (!skylights.empty())
				{
					auto entityID = skylights.front();
					Entity entity(entityID, this);
					auto& skyLightComponent = entity.GetComponent<SkyLightComponent>();
					UBSkyLight skyLightInfo;
					skyLightInfo.TintColor = skyLightComponent.ColorTint;
					skyLightInfo.Rotation = skyLightComponent.MapRotation;
					skyLightInfo.Intensity = skyLightComponent.Intensity;
					skyLightInfo.Lod = skyLightComponent.SkyBoxLoad;


					if (skyLightComponent.DynamicSky)
					{
						skyLightComponent.RemoveImage();
						auto environment = Renderer::CreatePreethamSky(skyLightComponent.Turbidity, skyLightComponent.Azimuth, skyLightComponent.Inclination);
						skyLightComponent.Environment = Count<Environment>::Create(environment, environment);
					}

					if (skyLightComponent.Image == 0 && skyLightComponent.Environment != nullptr && skyLightComponent.DynamicSky == false)
						skyLightComponent.Environment = nullptr;

					if (skyLightComponent.Environment != nullptr)
					{
						worldRenderer->SubmitSkyLight(skyLightInfo, skyLightComponent.Environment);
					}
				}
			}
			//point light
			{
				SBPointLightSceneData pointLightScene;
				auto pointLights = m_Registry.group<PointLightComponent>(entt::get<TransformComponent>);
				pointLightScene.PointLights.resize(pointLights.size());

				uint32_t pointLightIndex = 0;
				for (auto e : pointLights)
				{
					Entity entity(e, this);
					auto [transformComponent, pointLight] = pointLights.get<TransformComponent, PointLightComponent>(e);
					auto transform = GetWorldSpaceTransformComponent(entity);
					pointLightScene.PointLights[pointLightIndex] =
					{
							transform.Location,
							pointLight.Intensity,
							pointLight.Color,
							pointLight.MinRadius,
							pointLight.Radius,
							pointLight.Falloff,
							pointLight.CastsShadows,
							pointLight.SoftShadows,
							pointLight.ShadowStrength,
							pointLight.ShadowSoftness,
					};
					pointLightIndex++;
					if (pointLightIndex > 1)
						PF_ENGINE_INFO("{}",pointLightIndex);
				}
				if (!pointLights.empty())
				{
					worldRenderer->SubmitPointLight(pointLightScene);
				}
			}

			//spot lights
			{
				SBSpotLightSceneData spotLightSceneData;
				auto spotLights = m_Registry.group<SpotLightComponent>(entt::get<TransformComponent>);
				spotLightSceneData.SpotLights.resize(spotLights.size());
				uint32_t spotLightIndex = 0;
				for (auto e : spotLights)
				{
					Entity entity(e, this);
					auto [transformComponent, spotLight] = spotLights.get<TransformComponent, SpotLightComponent>(e);
					auto transform = GetWorldSpaceTransformComponent(entity);
					//auto transform = GetWorldSpaceTransform(entity);
					glm::vec3 direction = glm::normalize(glm::rotate(transform.GetRotation(), glm::vec3(1.0f, 0.0f, 0.0f)));
					//glm::vec3 direction = glm::normalize(glm::rotate(transform.GetRotation(), glm::vec3(1.0f, 0.0f, 0.0f)));

					spotLightSceneData.SpotLights[spotLightIndex++] = {
						transform.Location,
						spotLight.Intensity,
						direction,
						spotLight.AngleAttenuation,
						spotLight.Color,
						spotLight.Range,
						spotLight.Angle,
						spotLight.Falloff,
						spotLight.CastsShadows,
						spotLight.SoftShadows,
						spotLight.ShadowStrength,
						spotLight.ShadowSoftness,
					};
				}
				if (!spotLights.empty())
				{
					worldRenderer->SubmitSpotLight(spotLightSceneData);
				}
			}
		}
		#if 1
		// render meshes
		{
			auto group = m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				auto [transformComponent, staticMeshComponent] = group.get<TransformComponent, MeshComponent>(entity);
				if (!staticMeshComponent.Visible)
					continue;

				auto mesh = staticMeshComponent.GetMesh();
				if (mesh)
				{
					Entity e = Entity(entity, this);
					glm::mat4 transform = GetWorldSpaceTransform(e);

					//if (SelectionManager::IsEntityOrAncestorSelected(e))
					//	renderer->SubmitSelectedStaticMesh(entityUUID, staticMesh, staticMeshComponent.MaterialTable, transform);
					//else
					worldRenderer->SubmitMesh(mesh, staticMeshComponent.MaterialTable, transform, staticMeshComponent.CastShadow);
				}
			}

			auto dynamicGroup = m_Registry.group<DynamicMeshComponent>(entt::get<TransformComponent>);
			for (auto entity : dynamicGroup)
			{
				auto [transformComponent, dynamicMeshComponent] = dynamicGroup.get<TransformComponent, DynamicMeshComponent>(entity);
				if (!dynamicMeshComponent.Visible)
					continue;

				Count<DynamicMesh> mesh = dynamicMeshComponent.GetMesh();
				if (mesh)
				{
					Entity e = Entity(entity, this);
					glm::mat4 transform = GetWorldSpaceTransform(e);

					//if (SelectionManager::IsEntityOrAncestorSelected(e))
					//	renderer->SubmitSelectedStaticMesh(entityUUID, staticMesh, staticMeshComponent.MaterialTable, transform);
					//else
					worldRenderer->SubmitDynamicMesh(mesh, dynamicMeshComponent.MaterialTable, dynamicMeshComponent.GetSubMeshIndex(), transform,dynamicMeshComponent.CastShadow);
				}
			}
		}
		#endif
		RenderPhysicsDebug(worldRenderer, false);

		worldRenderer->EndScene();
		#endif
		// render 2d
		#if 0
		Count<Renderer2D> renderer2D = worldRenderer->GetRenderer2D();
		renderer2D->BeginContext(camera.GetProjectionMatrix(), camera.GetViewMatrix(), cameraLocation);

		renderer2D->SetTargetFrameBuffer(worldRenderer->GetExternalCompositePassFrameBuffer());

		auto view = m_Registry.view<TransformComponent, SpriteComponent>();
		for (auto entity : view)
		{
			Entity e = Entity(entity, this);
			auto [transformComponent, spriteRendererComponent] = view.get<TransformComponent, SpriteComponent>(entity);
			if (spriteRendererComponent.Texture)
			{
					//Count<Texture2D> texture = AssetManager::GetAsset<Texture2D>(spriteRendererComponent.Texture);
					renderer2D->DrawQuad(spriteRendererComponent, transformComponent);
			}
			else
			{
				renderer2D->DrawQuad(spriteRendererComponent, transformComponent);
			}
		}
		auto group = m_Registry.group<TransformComponent>(entt::get<TextComponent>);
		for (auto entity : group)
		{
			auto [transformComponent, textComponent] = group.get<TransformComponent, TextComponent>(entity);
			Entity e = Entity(entity, this);
			auto font = Font::GetDefault();

			TextParams params;
			params.Color = textComponent.Colour;
			params.Kerning = textComponent.Kerning;
			params.LineSpacing = textComponent.LineSpacing;
			if(textComponent.UseLocalRotation)
				renderer2D->DrawString(textComponent.Text, font, params, GetWorldSpaceTransformUsingLocalRotation(e));
			else
				renderer2D->DrawString(textComponent.Text, font, params, GetWorldSpaceTransform(e));
		}

		// render AABB
		{
			auto group = m_Registry.group<MeshComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				auto [transformComponent, staticMeshComponent] = group.get<TransformComponent, MeshComponent>(entity);
				if (!staticMeshComponent.Visible)
					continue;

				auto mesh = staticMeshComponent.GetMesh();
				if (mesh)
				{
					Entity e = Entity(entity, this);
					glm::mat4 transform = GetWorldSpaceTransform(e);

					//if (SelectionManager::IsEntityOrAncestorSelected(e))
					//	renderer->SubmitSelectedStaticMesh(entityUUID, staticMesh, staticMeshComponent.MaterialTable, transform);
					//else
					//renderer2D->DrawAABB(mesh,  transform);
					//renderer2D->DrawAABBSubMeshes(mesh, transform);
					//renderer2D->DrawAABB(mesh, transform);
					//AABB bouding = { { -1.0f, -1.0f, -1.0f} ,{1.0f, 1.0f, 1.0f} };
					//renderer2D->DrawAABB(bouding, transform);

				}
			}
		}
		//renderer2D->DrawLine({ 0,0,0 }, { 0,0,10 });
		renderer2D->EndContext();
		#endif

	}
	void World::RenderPhysicsDebug(Count<WorldRenderer> renderer, bool runtime)
	{
		if (renderer->Options.ShowPhysicsColliders == WorldRendererOptions::PhysicsColliderView::None)
			return;

		{
		
			auto view = m_Registry.view<BoxColliderComponent>();
			Count<Mesh> cubeMesh = PhysicsMeshCache::GetBoxColliderMesh();

			for (auto entity : view)
			{
				Entity e = { entity, this };
				glm::mat4 transform = GetWorldSpaceTransform(e);
				const auto& collider = e.GetComponent<BoxColliderComponent>();
				glm::mat4 colliderTransform = glm::translate(glm::mat4(1.0f), collider.Center) * glm::scale(glm::mat4(1.0f), collider.Size *2.0f);
				glm::mat4 finalTransform = transform * colliderTransform;
				renderer->SubmitPhysicsDebugMesh(cubeMesh, finalTransform);
			}
		}
		{
			auto view = m_Registry.view<SphereColliderComponent>();
			Count<Mesh> sphereDebugMesh = PhysicsMeshCache::GetSphereColliderMesh();
			for (auto entity : view)
			{
				Entity e = { entity, this };
				glm::mat4 transform = GetWorldSpaceTransform(e);
				const auto& collider = e.GetComponent<SphereColliderComponent>();
				glm::mat4 colliderTransform = glm::translate(glm::mat4(1.0), collider.Center) * glm::scale(glm::mat4(1.0f), glm::vec3(collider.Radius * 1.0f));
				renderer->SubmitPhysicsDebugMesh(sphereDebugMesh, transform * colliderTransform);
			}
		}
		{
			{
				auto view = m_Registry.view<CapsuleColliderComponent>();
				Count<Mesh> capsuleDebugMesh = PhysicsMeshCache::GetCapsuleColliderMesh();
				for (auto entity : view)
				{
					Entity e = { entity, this };
					glm::mat4 transform = GetWorldSpaceTransform(e);
					const auto& collider = e.GetComponent<CapsuleColliderComponent>();
					glm::mat4 colliderTransform = glm::mat4(1.0f); // Initialize with identity matrix

					if (collider.Direction == CapsuleDirection::X)
					{
						colliderTransform = glm::rotate(colliderTransform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate 90 degrees around X-axis
					}
					else if (collider.Direction == CapsuleDirection::Z)
					{
						colliderTransform = glm::rotate(colliderTransform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate 90 degrees around Z-axis
					}
					// Apply translation
					colliderTransform = glm::translate(colliderTransform, collider.Center);

					// Adjust scaling based on capsule direction
					float scale_x = (collider.Direction == CapsuleDirection::X) ? (collider.Height) : (collider.Radius * 2.0f);
					float scale_y = (collider.Direction == CapsuleDirection::Y) ? (collider.Height) : (collider.Radius * 2.0f);
					float scale_z = (collider.Direction == CapsuleDirection::Z) ? (collider.Height) : (collider.Radius * 2.0f);

					colliderTransform = glm::scale(colliderTransform, glm::vec3(scale_x, scale_y, scale_z));
					//glm::mat4 colliderTransform = glm::translate(glm::mat4(1.0), ProofToglmVec(collider.OffsetLocation)) * glm::scale(glm::mat4(1.0f), glm::vec3(collider.Radius * 2.0f, collider.Height, collider.Radius * 2.0f));
					renderer->SubmitPhysicsDebugMesh(capsuleDebugMesh, transform * colliderTransform);
				}
			}
		}

		{

			auto view = m_Registry.view<MeshColliderComponent>();
			for (auto entity : view)
			{
				Entity e = { entity, this };
				auto& collider = e.GetComponent<MeshColliderComponent>();
				Count<MeshCollider> colliderAsset = PhysicsEngine::GetOrCreateColliderAsset(e, collider);

				if (colliderAsset)
				{
					Count<Mesh> simpleDebugMesh = PhysicsMeshCache::GetDebugMesh(colliderAsset);
					if (simpleDebugMesh && colliderAsset->CollisionComplexity != ECollisionComplexity::UseSimpleAsComplex)
					{
						glm::mat4 transform = GetWorldSpaceTransform(e);
						renderer->SubmitPhysicsDebugMesh(simpleDebugMesh, transform);
					}
				}
			}
		}
	}
	void World::Init()
	{
	}

	void World::DeleteEntitiesfromQeue()
	{
		// job to remove entites does not care if has child or not
		for (auto& ID : m_EntityDeleteQueue)
		{
			// incase we only deleting the head enitty
			Entity entity = GetEntity(ID);
			entity.EachChild([&](Entity childEntity) {
				childEntity.Unparent();
			});
			if (entity.HasParent())
			{
				if (TryGetEntityWithUUID(entity.GetParentUUID()))
				{
					GetEntity((entity.GetParentUUID())).RemoveChild(entity);
				}
			}
			m_Registry.destroy((entt::entity)entity);
			m_EntitiesMap.erase(ID);
		}

		m_EntityDeleteQueue.clear();
	}

	void World::OnMeshColliderComponentCreate(MeshColliderComponent& component)
	{
		//PF_CORE_ASSERT(false);
	}

	void World::OnMeshColliderComponentDelete(MeshColliderComponent& component)
	{

	}

	void World::OnRigidBodyComponentCreate(entt::registry& component, entt::entity entityID)
	{
		Entity e = { entityID, this };
		m_PhysicsWorld->CreateActor(e);
	}


	void World::OnRigidBodyComponentDelete(entt::registry& component, entt::entity entityID)
	{
		Entity e = { entityID, this };

		m_PhysicsWorld->RemoveActor(e);
	}

	void World::OnScriptAdded(entt::registry& component, entt::entity entityID)
	{
		//ScriptMeathod::OnCreate({ entityID,this });
	}

	void World::OnScriptDelete(entt::registry& component, entt::entity entityID)
	{
		//ScriptMeathod::OnDestroy({ entityID,this }); 
	}
	void World::BuildDynamicMeshEntityHierarchy(Entity parent, Count<DynamicMesh> mesh, const MeshNode& node, bool generateColliders)
	{
		Count<MeshSource> meshSource = mesh->GetMeshSource();
		const auto& nodes = meshSource->GetNodes();

		// Skip empty root node
		if (node.IsRoot() && node.Submeshes.size() == 0)
		{
			for (uint32_t child : node.Children)
				BuildDynamicMeshEntityHierarchy(parent, mesh, nodes[child], generateColliders);

			return;
		}

		Entity nodeEntity = CreateChildEntity(parent, node.Name);
		nodeEntity.Transform().SetTransform(node.LocalTransform);

		if (node.Submeshes.size() == 1)
		{
			// Node == Mesh in this case
			uint32_t submeshIndex = node.Submeshes[0];

			nodeEntity.AddComponent<DynamicMeshComponent>();
			DynamicMeshComponent& component = nodeEntity.GetComponent< DynamicMeshComponent>();
			component.SetMesh(mesh->GetID());
			component.SetSubMeshIndex(submeshIndex);

			/*
			if (generateColliders)
			{
				auto& colliderComponent = nodeEntity.AddComponent<MeshColliderComponent>();
				Ref<MeshColliderAsset> colliderAsset = PhysicsSystem::GetOrCreateColliderAsset(nodeEntity, colliderComponent);
				colliderComponent.ColliderAsset = colliderAsset->Handle;
				colliderComponent.SubmeshIndex = submeshIndex;
				colliderComponent.UseSharedShape = colliderAsset->AlwaysShareShape;
				nodeEntity.AddComponent<RigidBodyComponent>();
			}
			*/
		}
		else if (node.Submeshes.size() > 1)
		{
			// Create one entity per child mesh, parented under node
			for (uint32_t i = 0; i < node.Submeshes.size(); i++)
			{
				
				uint32_t submeshIndex = node.Submeshes[i];
				Entity childEntity = CreateChildEntity(nodeEntity, meshSource->GetSubMesh(submeshIndex).Name);

				childEntity.AddComponent<DynamicMeshComponent>();
				DynamicMeshComponent& component = childEntity.GetComponent< DynamicMeshComponent>();
				component.SetMesh(mesh->GetID());
				component.SetSubMeshIndex(submeshIndex);
				/*
				if (generateColliders)
				{
					auto& colliderComponent = childEntity.AddComponent<MeshColliderComponent>();
					Ref<MeshColliderAsset> colliderAsset = PhysicsSystem::GetOrCreateColliderAsset(childEntity, colliderComponent);
					colliderComponent.ColliderAsset = colliderAsset->Handle;
					colliderComponent.SubmeshIndex = submeshIndex;
					colliderComponent.UseSharedShape = colliderAsset->AlwaysShareShape;
					childEntity.AddComponent<RigidBodyComponent>();
				}
				*/
			}

		}

		for (uint32_t child : node.Children)
			BuildDynamicMeshEntityHierarchy(nodeEntity, mesh, nodes[child], generateColliders);
	}
	void World::OnUpdateRuntime(FrameTime DeltaTime) 
	{
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
		if (m_CurrentState == WorldState::Pause)
			return;
		{
			PF_PROFILE_FUNC("World::OnUpdate - Particle OnUpdate");

			const auto& particleView = m_Registry.view<ParticleSystemComponent>();
			for (auto entity : particleView)
			{
				auto& particleSystem = particleView.get<ParticleSystemComponent>(entity);
				if (particleSystem.ParticleHandlerTable == nullptr)continue;
				Entity wfadfas{ entity,this };
				for (auto& [index, handler] : particleSystem.ParticleHandlerTable->GetHandlers())
				{
					if (handler != nullptr)
					{
						if(handler->Visible == true)
							handler->Update(DeltaTime, GlmVecToProof( GetWorldSpaceLocation(wfadfas)));
					}
				}
			}
		}

		{
			PF_PROFILE_FUNC("World::OnUpdate - C# OnUpdate");

			const auto& scriptView = m_Registry.view<ScriptComponent>();
			for (auto entity : scriptView)
			{
				auto& script = scriptView.get<ScriptComponent>(entity);
				//ScriptMeathod::OnUpdate(Entity{ entity,this }, DeltaTime);
			}
		}
		

		{
			PF_PROFILE_FUNC("World::OnUpdate - Audio");
			{
				PF_PROFILE_FUNC("World::OnUpdate - AudioListener");
				//m_Camera

				Entity listener;
				auto view = m_Registry.view<AudioListenerComponent>();
				for (auto e : view)
				{
					Entity check = { e,this };
					auto& listenerComponent = check.GetComponent<AudioListenerComponent>();

					if (listenerComponent.Active)
					{
						listener = check;

						auto transform = Utils::TransformToAudioTransform(GetWorldSpaceTransformComponent(listener));
						
						AudioEngine::UpdateListenerPosition(transform);
						AudioEngine::UpdateListenerConeAttenuation(listenerComponent.ConeInnerAngleInRadians, listenerComponent.ConeOuterAngleInRadians, listenerComponent.ConeOuterGain);
						auto physicsActor = m_PhysicsWorld->GetActor(listener);
						if (physicsActor)
						{

							if(physicsActor->IsDynamic())
								AudioEngine::UpdateAudioListenerVelocity( physicsActor->GetLinearVelocity());
						}
						else
						{
							AudioEngine::UpdateAudioListenerVelocity(glm::vec3{ 0 });
						}
						break;

					}
				}

				// we are going to remove this soon just for now testing
				if (!listener)
				{
					TransformComponent comp;
					comp.Location = m_CameraPositon;
					auto transform = Utils::TransformToAudioTransform(comp);
					AudioEngine::UpdateListenerPosition(transform);

					AudioListenerComponent defaultSettings;
					AudioEngine::UpdateListenerConeAttenuation(defaultSettings.ConeInnerAngleInRadians, defaultSettings.ConeOuterAngleInRadians, defaultSettings.ConeOuterGain);
				}
			}


			{
				PF_PROFILE_FUNC("World::OnUpdate - Audio");
				auto view = m_Registry.view<AudioComponent>();

				for (auto e : view)
				{

					Entity audioEntity = { e,this };
					auto& audioComponent = audioEntity.GetComponent<AudioComponent>();
					auto transform = Utils::TransformToAudioTransform(GetWorldSpaceTransformComponent(audioEntity));

					AudioEngine::UpdateAudio(audioEntity.GetUUID(), Utils::AudioComponentToSoundConfig(audioComponent));
					AudioEngine::UpdateAudioTransform(audioEntity.GetUUID(), transform);
					auto physicsActor = m_PhysicsWorld->GetActor(audioEntity);

					if (physicsActor)
					{
						if(physicsActor->IsDynamic())
							AudioEngine::UpdateAudioVelocity(audioEntity.GetUUID(), physicsActor->GetLinearVelocity());
					}
					else
					{
						AudioEngine::UpdateAudioVelocity(audioEntity.GetUUID(), glm::vec3{ 0 });
					}
				}
			}
		}
		if (HasWorldCamera())
		{
		}
		m_PhysicsWorld->Simulate(DeltaTime);

		DeleteEntitiesfromQeue();
	}

	Entity World::TryGetEntityWithUUID(UUID id)const {
		if (const auto iter = m_EntitiesMap.find(id); iter != m_EntitiesMap.end())
			return iter->second;
		return Entity{};
	}
	Entity World::GetEntity(UUID id)
	{
		PF_CORE_ASSERT(m_EntitiesMap.find(id) != m_EntitiesMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
		return m_EntitiesMap.at(id);
	}

	void World::OnSimulatePhysics(FrameTime DeltaTime) {
		DeleteEntitiesfromQeue();
	}

	bool World::HasWorldCamera() {
		const auto& cameraGroup = m_Registry.group<TransformComponent>(entt::get<CameraComponent>);
		return cameraGroup.size() > 0;
	}

	Entity World::GetWorldCameraEntity() {
		if (HasWorldCamera() == false)return Entity{};
		const auto& cameraGroup = m_Registry.group<TransformComponent>(entt::get<CameraComponent>);
		for (auto entity : cameraGroup)
		{
			return Entity{ entity,this };
		}
	}

	void World::OnRenderEditor(Count<class WorldRenderer> renderer, FrameTime time, const EditorCamera& camera)
	{
		
		OnRender(renderer, time, camera,GlmVecToProof( camera.GetPosition()), camera.GetNearPlane(), camera.GetFarPlane());
	}

	Entity World::CreateEntity(const std::string& EntName) {
		return CreateEntity(EntName, UUID());
	}

	Entity World::CreateEntity(const std::string& EntName, UUID ID) {
		/* we have to do some custmization of entt because when we pass an ID the entities create a vecot of the size of ID*/

		Entity entity = { m_Registry.create(),this };

		entity.AddComponent<IDComponent>(ID);
		entity.AddComponent<TagComponent>().Tag = EntName;
		entity.AddComponent<HierarchyComponent>();
		entity.AddComponent<TransformComponent>();

		m_EntitiesMap[ID] = entity;
		return entity;
	}

	template<typename... Componnents>
	static void CopyComponentIfExistEntitySingle(Entity dst, Entity src, bool isdstPrefab = false, bool isSrcPrefab = false)
	{
		([&]()
		{
			if ((typeid(Componnents) == typeid(IDComponent) || typeid(Componnents) == typeid(HierarchyComponent)) )
			{
				return;
			}

			if (src.HasComponent<Componnents>())
				dst.AddorReplaceComponent<Componnents>(src.GetComponent<Componnents>());
			else
				return;
			if(typeid(Componnents) == typeid(ScriptComponent))
				dst.GetCurrentWorld()->GetScriptWorld()->DuplicateScriptInstance(src, dst);

		}(), ...);
	}
	template<typename... Component>
	static void CopyComponentIfExistsEntity(ComponentGroup<Component...>, Entity dst, Entity src, bool isdstPrefab = false, bool isSrcPrefab = false)
	{
		//if (isdstPrefab == true && isSrcPrefab == true)
		//{
		//	PF_CORE_ASSERT(false);
		//}
		CopyComponentIfExistEntitySingle<Component...>(dst, src, isdstPrefab, isSrcPrefab);
	}

	//src entity is used as childenitty in resurio
	void World::PrefabCopyEntity(Count<class Prefab> prefab, Entity srcEntity, Entity parentEntity,bool includeChildren)
	{
		// first function call for base has to be null
		// handle it

		CopyComponentIfExistsEntity(AllComponents{}, parentEntity, srcEntity, true);

		if (includeChildren )
		{
			srcEntity.EachChild([&](Entity childEntity)
			{
				Entity newEntity;
				newEntity = prefab->m_World->CreateEntity();
				newEntity.SetParent(parentEntity);
				//Entity newChild = prefab->m_World->CreateEntity(childEntity, true);
				PrefabCopyEntity(prefab, childEntity, newEntity,true);
			});
		}
	}

	Entity World::CreateEntity(Entity entity, bool includeChildren) {
		Entity newEntity = CreateEntity(entity.GetName());
		CopyComponentIfExistsEntity (AllComponents{},newEntity,entity);

		if (includeChildren == true)
		{
			entity.EachChild([&](Entity childEntity) {
				Entity newChild = CreateEntity(childEntity, true);
				newEntity.AddChild(newChild);
			});
		}
		return newEntity;
	}
	Entity World::CreateEntity(Count<class DynamicMesh> mesh, bool generateCollider)
	{
		PF_CORE_ASSERT(mesh->GetID());
		auto info = AssetManager::GetAssetInfo(mesh);
		Entity root = CreateEntity(info.GetName());
		BuildDynamicMeshEntityHierarchy(root, mesh, mesh->GetMeshSource()->GetRootNode(), generateCollider);
		return root;
	}
	Entity World::CreateChildEntity(Entity parent, const std::string& name)
	{
		PF_PROFILE_FUNC();
		Entity entity = CreateEntity(name);
		if (parent)
			entity.SetParent(parent);
		return entity;
	}
	#if 0
	template<typename... Component>
	static void CopyComponentSinglPrefab(Entity dstEntity, entt::registry& dstMap,const entt::registry& src, UUID srcID,const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
		{
			if (typeid(Component) == typeid(IDComponent))
			{
				return;
			}

			entt::entity destinationID = (entt::entity)dstEntity;
			if (!src.all_of<Component>(srcID))
				return;

			const auto& srcComponent = src.get<Component>(srcID);
			dstMap.emplace_or_replace<Component>(destinationID, srcComponent);
			
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponentPrefab(ComponentGroup<Component...>, Entity dst, entt::registry& dstMap,const entt::registry& src,UUID srcID, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponentSinglPrefab<Component...>(dst, dstMap, src, srcID, enttMap);
	}
	#endif

	static Entity CreateEntityPrefabStatic(Count<Prefab> prefab, World* world, Entity prefabSource, bool includeChildren = true)
	{
		Entity newEntity = world->CreateEntity();
		CopyComponentIfExistsEntity(AllComponents{}, newEntity, prefabSource, false, true);
		newEntity.AddComponent<PrefabComponent>();
		newEntity.GetComponent<PrefabComponent>().PrefabID = prefab->GetID();
		newEntity.GetComponent<PrefabComponent>().PrefabEntityID = prefabSource.GetUUID();

		if (includeChildren)
		{
			prefabSource.EachChild([&](Entity child) {
				Entity newChild = CreateEntityPrefabStatic(prefab, world, child, true);
				newChild.SetParent(newEntity);
			});
		}
		return newEntity;
	}
	Entity World::CreateEntity(const std::string& name, Count<Prefab> prefab, TransformComponent transfom, UUID id)
	{
		PF_PROFILE_FUNC();
		/**
		 * when an entity with sub children has rigid body this fucntions crahses 
		 * it could be a problem with the emplace or replace in the copy compoentnt single
		 * 
		 */
		Entity prefabBaseEntity = prefab->m_BaseEntity;
		if (!prefabBaseEntity)return {};
		Entity newEntity = CreateEntityPrefabStatic(prefab, this, prefabBaseEntity, true);
		newEntity.SetName(name);
		newEntity.GetComponent<TransformComponent>() = transfom;
		return newEntity;
	}
	template<typename... Component>
	static void CopyComponentSingleWorld(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
		{
			if (typeid(Component) == typeid(IDComponent))
			{
				return;
			}
			auto view = src.view<Component>();
			for (auto srcEntity : view)
			{
				UUID id = src.get<IDComponent>(srcEntity).GetID();
				entt::entity dstEntity = enttMap.at(id);

				auto& srcComponent = src.get<Component>(srcEntity);
				dst.emplace_or_replace<Component>(dstEntity, srcComponent);

				//dst.GetCurrentWorld()->GetScriptWorld()->DuplicateScriptInstance(src, dst);
			}
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponentSingleWorld<Component...>(dst, src, enttMap);
	}

	//static void CopyComponent
	Count<World> World::Copy(Count<World> worldToCopy) {
		Count<World> newWorld = Count<World>::Create();
		
		newWorld->Name = worldToCopy->Name;
		///newWorld->m_WorldID = other->m_WorldID;
	
		auto& srcSceneRegistry = worldToCopy->m_Registry;
		auto& dstSceneRegistry = newWorld->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create entities in new scene
		// in reverse order
		auto idView = srcSceneRegistry.view<IDComponent>();
		std::for_each(idView.rbegin(), idView.rend(), [&](auto e) {
			EntityID uuid = srcSceneRegistry.get<IDComponent>(e).GetID();
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newWorld->CreateEntity(name, uuid);
			enttMap[uuid] = (entt::entity)newEntity;
		});

		// Copy components (except IDComponent )
		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		newWorld->m_ScriptWorld = ScriptWorld::CopyScriptWorld(worldToCopy->GetScriptWorld(), newWorld);

		return newWorld;
	}

	

	void World::StartRuntime() {
		m_CurrentState = WorldState::Play;
		int numPlayrs = 1;
		numPlayrs += Application::Get()->GetWindow()->GetControllers().size();
		InputManager::StartRuntime(numPlayrs);

		ForEachEnitityWith<PlayerInputComponent>([&](Entity entity) {
			PlayerInputComponent inputCopy = entity.GetComponent<PlayerInputComponent>();
			if (inputCopy.InputPlayer == Players::None)
				return;
			TransformComponent transfomr = entity.GetComponent<TransformComponent>();
			if (!AssetManager::HasAsset(inputCopy.Player))return;
			if ((int)inputCopy.InputPlayer > numPlayrs)
			{
				UUID playerID = entity.GetUUID();
				DeleteEntity(entity);
				DeleteEntitiesfromQeue();
				return;
			}
			UUID playerID = entity.GetUUID();
			DeleteEntity(entity);
			DeleteEntitiesfromQeue();

			Entity newEntity =CreateEntity(AssetManager::GetAssetInfo(inputCopy.Player).GetName(), inputCopy.Player, transfomr, playerID);
			newEntity.AddComponent<PlayerInputComponent>(inputCopy);

			InputManagerMeathods::SetPlayer((uint32_t)inputCopy.InputPlayer);
		});
		ForEachEnitityWith<PlayerHUDComponent>([&](Entity entity) {
			PlayerHUDComponent& hud = entity.GetComponent<PlayerHUDComponent>();
			if (hud.HudTable != nullptr)
			{
				hud.HudTable = Count<UITable>::Create(hud.HudTable->Generate());
			}
		});
		ForEachEnitityWith<ParticleSystemComponent>([&](Entity entity) {
			ParticleSystemComponent& part = entity.GetComponent<ParticleSystemComponent>();
			if (part.ParticleHandlerTable != nullptr)
			{
				part.ParticleHandlerTable = Count<ParticleHandlerTable>::Create(part.ParticleHandlerTable->Generate());
			}
		});
		m_Registry.on_construct<ScriptComponent>().connect<&World::OnScriptAdded>(this);
		m_Registry.on_destroy<ScriptComponent>().connect<&World::OnScriptDelete>(this);
		{
			const auto& scriptView = m_Registry.view<NativeScriptComponent>();
			for (auto entity : scriptView)
			{
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
				m_ScriptWorld->BeginRuntime();
				//auto view = m_Registry.view<ScriptComponent>();
				//for (auto e : view)
				//{
				//	Entity entity = { e, this };
				//	//ScriptMeathod::OnCreate(entity);
				//	//ScriptEngine::OnPlace(entity);
				//}
			}
		}
		///
		///
		//PhysicsWorldConfig config;
		//config.PvdClient = true;
		//config.Gravity = { 0,-9.8f,0 };// for multiplayer scene
		m_PhysicsWorld = Count<PhysicsWorld>::Create (this);
		m_Registry.on_construct<RigidBodyComponent>().connect<&World::OnRigidBodyComponentCreate>(this);
		m_Registry.on_destroy<RigidBodyComponent>().connect < &World::OnRigidBodyComponentDelete>(this);
		
		Count<World> instance = this;
		AudioEngine::BeginContext(instance);
	}
	void World::EndRuntime() {

		InputManager::EndRuntime();

		m_ScriptWorld->EndRuntime();

		m_Registry.on_construct<RigidBodyComponent>().disconnect(this);
		m_Registry.on_destroy<RigidBodyComponent>().disconnect(this);

		m_Registry.on_construct<ScriptComponent>().disconnect(this);
		m_Registry.on_destroy<ScriptComponent>().disconnect(this);
		
		AudioEngine::EndContext();
		ScriptEngine::EndRuntime();
		m_PhysicsWorld =nullptr;
	}
	void World::DeleteEntity(Entity ent, bool deleteChildren) {
		if(!m_EntitiesMap.contains(ent.GetUUID()))
			return;
		m_EntityDeleteQueue.insert(ent.GetUUID());
		if (deleteChildren)
		{
			ent.EachChild([&](Entity childEntity) {
				DeleteEntity(childEntity, true);
			});
		}

	}

	void World::ConvertToWorldSpaceTransform(Entity entity)
	{
		PF_PROFILE_FUNC();

		Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());

		if (!parent)
			return;

		glm::mat4 transform = GetWorldSpaceTransform(entity);
		auto& entityTransform = entity.GetComponent<TransformComponent>();
		entityTransform.SetTransform(transform);
	}

	Entity World::TryGetEntityByTag(const std::string& tag) {
		Entity returnEntity;
		auto entities = GetAllEntitiesWith<TagComponent>();
		for (auto e : entities)
		{
			if (entities.get<TagComponent>(e).Tag == tag)
				return Entity(e, const_cast<World*>(this));
		}

		return Entity{};
	}

	glm::vec3 World::GetWorldSpaceLocation(Entity entity) const {
		return GetWorldSpaceTransformComponent(entity).Location;

		auto& transformComp = entity.GetComponent<TransformComponent>();
		if (entity.HasParent())
			return transformComp.Location + GetWorldSpaceLocation(entity.GetParent());
		return transformComp.Location;
	}

	glm::vec3 World::GetWorldSpaceRotation(Entity entity) const {
		return GetWorldSpaceTransformComponent(entity).GetRotationEuler();
	}

	glm::vec3 World::GetWorldSpaceScale(Entity entity) const 
	{
		return GetWorldSpaceTransformComponent(entity).Scale;
	}

	TransformComponent World::GetWorldSpaceTransformComponent(Entity entity) const
	{
		glm::mat4 transform = GetWorldSpaceTransform(entity);
		TransformComponent transformComponent;
		transformComponent.SetTransform(transform);
		return transformComponent;
	}
	
	glm::mat4 World::GetWorldSpaceTransform(Entity entity) const {

		//auto rotation = GetWorldSpaceRotation(entity);
		//return glm::translate(glm::mat4(1.0f), { GetWorldSpaceLocation(entity) }) *
		//	glm::rotate(glm::mat4(1.0f), rotation.x, { 1,0,0 })
		//	* glm::rotate(glm::mat4(1.0f), rotation.y, { 0,1,0 })
		//	* glm::rotate(glm::mat4(1.0f), rotation.z, { 0,0,1 })
		//	* glm::scale(glm::mat4(1.0f), { GetWorldSpaceScale(entity)});
		glm::mat4 transform(1.0f);
		Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());
		if (parent)
			transform = GetWorldSpaceTransform(parent);
		
		return transform * entity.Transform().GetTransform();
	}

	glm::mat4 World::GetWorldSpaceTransformUsingLocalRotation(Entity entity) const
	{
		auto rotation = entity.GetComponent<TransformComponent>().GetRotation();
		return glm::translate(glm::mat4(1.0f), { GetWorldSpaceLocation(entity) }) *
			glm::rotate(glm::mat4(1.0f), rotation.x, { 1,0,0 })
			* glm::rotate(glm::mat4(1.0f), rotation.y, { 0,1,0 })
			* glm::rotate(glm::mat4(1.0f), rotation.z, { 0,0,1 })
			* glm::scale(glm::mat4(1.0f), { GetWorldSpaceScale(entity) });
	}

	TransformComponent World::GetWorldSpaceTransformComponentUsingLocalRotation(Entity entity) const
	{

		glm::mat4 transform = GetWorldSpaceTransformUsingLocalRotation(entity);
		TransformComponent transformComponent;
		transformComponent.SetTransform(transform);
		return transformComponent;
	}

	void World::ConvertToLocalSpace(Entity entity)
	{
		PF_PROFILE_FUNC();

		Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());

		if (!parent)
			return;

		auto& transform = entity.Transform();
		glm::mat4 parentTransform = GetWorldSpaceTransform(parent);
		glm::mat4 localTransform = glm::inverse(parentTransform) * transform.GetTransform();
		transform.SetTransform(localTransform);
	}

	void World::ParentEntity(Entity entity, Entity parent)
	{
		PF_PROFILE_FUNC();
		if (entity.GetParent() == parent)
			return;

		if (!parent)
			return;
		if (parent.IsDescendantOf(entity))
		{
			UnparentEntity(parent);

			Entity newParent = TryGetEntityWithUUID(entity.GetParentUUID());
			if (newParent)
			{
				UnparentEntity(entity);
				ParentEntity(parent, newParent);
			}
		}
		else
		{
			Entity previousParent = TryGetEntityWithUUID(entity.GetParentUUID());

			if (previousParent)
				UnparentEntity(entity);
		}
		entity.GetComponent<HierarchyComponent>().ParentHandle = parent.GetUUID();
		parent.Children().push_back(entity.GetUUID());

		ConvertToLocalSpace(entity);
	}	

	void World::UnparentEntity(Entity entity, bool convertToWorldSpace)
	{
		PF_PROFILE_FUNC();
		Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());
		if (!parent)
			return;
		auto& parentChildren = parent.Children();
		parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), entity.GetUUID()), parentChildren.end());

		if (convertToWorldSpace)
			ConvertToWorldSpaceTransform(entity);

		entity.GetComponent<HierarchyComponent>().ParentHandle = 0;
	}

}