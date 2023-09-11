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
#include "Physics/PhysicsEngine.h"
#include "Physics/PhysicsWorld.h"
#include "Proof/Input/InputManager.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Scene/Prefab.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Core/Application.h"

namespace Proof {
	World::World(const std::string& name, UUID ID):
		Name(name)
	{
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
					directionaLightScene.DirectionalLights[index].Direction = -GlmVecToProof(direction);
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
					worldRenderer->SubmitStaticMesh(mesh, staticMeshComponent.MaterialTable, transform, staticMeshComponent.CastShadow);
				}
			}
		}

		worldRenderer->EndScene();
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
		PF_CORE_ASSERT(false);
	}

	void World::OnMeshColliderComponentDelete(MeshColliderComponent& component)
	{

	}

	void World::OnRigidBodyComponentCreate(entt::registry& component, entt::entity entityID)
	{
		Entity e = { entityID, this };
		m_PhysicsWorld->NewActor(e.GetUUID());
	}


	void World::OnRigidBodyComponentDelete(entt::registry& component, entt::entity entityID)
	{
		Entity e = { entityID, this };

		m_PhysicsWorld->RemoveActor(e.GetUUID());
	}

	void World::OnScriptAdded(entt::registry& component, entt::entity entityID)
	{
		ScriptMeathod::OnCreate({ entityID,this });
	}

	void World::OnScriptDelete(entt::registry& component, entt::entity entityID)
	{
		ScriptMeathod::OnDestroy({ entityID,this }); 
	}
	void World::OnUpdateRuntime(FrameTime DeltaTime) {
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
			PF_PROFILE_FUNC("World::OnUpdateEditor - C# OnUpdate");

			const auto& scriptView = m_Registry.view<ScriptComponent>();
			for (auto entity : scriptView)
			{
				auto& script = scriptView.get<ScriptComponent>(entity);
				ScriptMeathod::OnUpdate(Entity{ entity,this }, DeltaTime);
			}
		}
		
		if (HasWorldCamera())
		{
		}
		m_PhysicsWorld->OnFixedUpdate(DeltaTime);

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
			if (typeid(Componnents) == typeid(ScriptComponent) && (isdstPrefab == false || isSrcPrefab == false))
			{
				if (!src.HasComponent<ScriptComponent>()) return;

				auto& scriptComponent = src.GetComponent<ScriptComponent>();
				for (auto& scripts : scriptComponent.ScriptsNames)
				{
					if (ScriptEngine::HasScriptFieldMap(src) == false)continue;
					if(!ScriptEngine::HasScriptFieldMap(dst))
						ScriptEngine::CreateScriptFieldMap(dst);

					ScriptEngine::GetScriptFieldMap(dst) = ScriptEngine::GetScriptFieldMap(src);
				}
			}
		}(), ...);
	}
	template<typename... Component>
	static void CopyComponentIfExistsEntity(ComponentGroup<Component...>, Entity dst, Entity src, bool isdstPrefab = false, bool isSrcPrefab = false)
	{
		if (isdstPrefab == true && isSrcPrefab == true)
		{
			PF_CORE_ASSERT(false);
		}
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
		return newEntity;
		//
		//std::unordered_map<UUID, entt::entity> enttMap;
		//{
		//
		//
		//	enttMap[prefab->GetBaseEntity()] = (entt::entity)newEntity;
		//
		//	//CopyComponentPrefab(AllComponents{}, newEntity, m_Registry, prefab->GetRegistry(), prefab->GetBaseEntity(), enttMap);
		//	newEntity.GetComponent<TransformComponent>() = transfom;
		//	
		//
		//	if (prefab->GetRegistry().size() == 1)
		//		return newEntity;
		//}
		#if 0
		auto& prefaRegistry = prefab->GetRegistry();
		std::function<Entity(entt::entity)> createEntity = [&](entt::entity registryEntityId)->Entity
		{
			if (prefaRegistry.get<HierarchyComponent>(registryEntityId).ParentHandle != 0)
			{
				Entity owner;
				UUID owenrID = prefaRegistry.get<HierarchyComponent>(registryEntityId).ParentHandle;
				if (!enttMap.contains(owenrID))
					owner = createEntity(owenrID);
				else
					owner = { enttMap[owenrID],this };

				Entity thisEntity = CreateEntity();
				enttMap[registryEntityId] = thisEntity.GetUUID();
				CopyComponentPrefab(AllComponents{}, thisEntity, m_Registry, prefab->GetRegistry(), prefaRegistry.get<IDComponent>(registryEntityId).GetID(), enttMap);
			
				for (UUID id : prefaRegistry.get<HierarchyComponent>(registryEntityId).Children)
				{
					if (enttMap.contains(registryEntityId))
					{
						Entity subChild = Entity{ enttMap[registryEntityId], this };
						thisEntity.AddChild(subChild);
					}
					else
					{
						createEntity(id);
					}
				}
				if(owner)
					owner.GetComponent<HierarchyComponent>().Children.push_back(thisEntity.GetUUID());
				return thisEntity;
			}
			else
			{
				Entity basicEntity = CreateEntity();
				enttMap[registryEntityId] = basicEntity.GetUUID();
				CopyComponentPrefab(AllComponents{}, basicEntity, m_Registry, prefab->GetRegistry(), registryEntityId, enttMap);

				return basicEntity;
			}
		};
		
		for (UUID registryEntityID : prefab->GetRegistry().)
		{
			if (registryEntityID == prefab->GetBaseEntity())continue;
			if (enttMap.contains(registryEntityID))
				continue;

			createEntity(registryEntityID);
		}
		#endif
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
				ScriptEngine::BeginRuntime(this);
				auto view = m_Registry.view<ScriptComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					ScriptMeathod::OnCreate(entity);
					//ScriptEngine::OnPlace(entity);
				}
			}
		}
		///
		///
		PhysicsWorldConfig config;
		config.Gravity = { 0,-9.8f,0 };// for multiplayer scene
		m_PhysicsWorld = new PhysicsWorld(this, config);
		m_Registry.on_construct<RigidBodyComponent>().connect<&World::OnRigidBodyComponentCreate>(this);
		m_Registry.on_destroy<RigidBodyComponent>().connect < &World::OnRigidBodyComponentDelete>(this);


	}
	void World::EndRuntime() {
		InputManager::EndRuntime();

		m_Registry.on_construct<RigidBodyComponent>().disconnect(this);
		m_Registry.on_destroy<RigidBodyComponent>().disconnect(this);

		m_Registry.on_construct<ScriptComponent>().disconnect(this);
		m_Registry.on_destroy<ScriptComponent>().disconnect(this);

		ScriptEngine::EndRuntime();
		delete m_PhysicsWorld;
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
		//return GetWorldSpaceTransformComponent(entity).Location;

		auto& transformComp = entity.GetComponent<TransformComponent>();
		if (entity.HasParent())
			return transformComp.Location + GetWorldSpaceLocation(entity.GetParent());
		return transformComp.Location;
	}

	glm::vec3 World::GetWorldSpaceRotation(Entity entity) const {
		//return GetWorldSpaceTransformComponent(entity).GetRotationEuler();
		auto& transformComp = entity.GetComponent<TransformComponent>();
		if (entity.HasParent())
			return transformComp.GetRotationEuler() + GetWorldSpaceRotation(entity.GetParent());
		return transformComp.GetRotationEuler();
	}

	glm::vec3 World::GetWorldSpaceScale(Entity entity) const 
	{
		auto& transformComp = entity.GetComponent<TransformComponent>();
		if (entity.HasParent())
			return transformComp.Scale * World::GetWorldSpaceScale(entity.GetParent());
		return transformComp.Scale;
	}

	TransformComponent World::GetWorldSpaceTransformComponent(Entity entity) const
	{
		TransformComponent transform;
		transform.Location = GetWorldSpaceLocation(entity);
		transform.SetRotationEuler(GetWorldSpaceRotation(entity));
		transform.Scale = GetWorldSpaceScale(entity);

		return transform;
		//glm::mat4 transform = GetWorldSpaceTransform(entity);
		//TransformComponent transformComponent;
		//transformComponent.SetTransform(transform);
		//return transformComponent;
	}
	
	glm::mat4 World::GetWorldSpaceTransform(Entity entity) const {

		auto rotation = GetWorldSpaceRotation(entity);
		return glm::translate(glm::mat4(1.0f), { GetWorldSpaceLocation(entity) }) *
			glm::rotate(glm::mat4(1.0f), rotation.x, { 1,0,0 })
			* glm::rotate(glm::mat4(1.0f), rotation.y, { 0,1,0 })
			* glm::rotate(glm::mat4(1.0f), rotation.z, { 0,0,1 })
			* glm::scale(glm::mat4(1.0f), { GetWorldSpaceScale(entity)});
		//glm::mat4 transform(1.0f);
		//Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());
		//if (parent)
		//	transform = GetWorldSpaceTransform(parent);
		//
		//return transform * entity.Transform().GetTransform();
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