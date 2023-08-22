#include "Proofprch.h"
#include "World.h"
#include "Component.h"
#include "Entity.h"
#include "Script.h"
#include "Mesh.h"
#include "Proof/Core/FrameTime.h"
#include "Component.h"
#include "Proof/Renderer/3DRenderer/Renderer3D.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Scene/Component.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/VertexArray.h"
#include "Proof/Renderer/FrameBuffer.h"
#include<glad/glad.h>
#include "Proof/Scene/Component.h"
#include "entt/entt.hpp"
#include "Proof/Scripting/ScriptEngine.h"
#include "Physics/PhysicsEngine.h"
#include "Physics/PhysicsWorld.h"
#include "Proof/Input/InputManager.h"
#include "Proof/Scene/Prefab.h"
namespace Proof {
	World::World(const std::string& name, UUID ID)
		:
		m_WorldID(ID), Name(name)
	{
		Init();

		//m_Registry.on_destroy<ChildComponent>().connect<&World::OnChildComponentDestroy>(this);

	}
	World::~World()
	{
		m_Registry = {};
	}
	bool World::HasEntity(EntityID ID)const {
		auto it = std::find(m_Registry.entities.begin(), m_Registry.entities.end(), ID.Get());
		if (it == m_Registry.entities.end())
			return false;
		return true;
	}
	bool World::HasEntity(EntityID ID) {
		auto it = std::find(m_Registry.entities.begin(), m_Registry.entities.end(), ID.Get());
		if (it == m_Registry.entities.end())
			return false;
		return true;
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
							handler->Update(DeltaTime, GetWorldLocation(wfadfas));
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
			auto it = std::find(m_Registry.entities.begin(), m_Registry.entities.end(), ID.Get());
			if (it == m_Registry.entities.end())
				return;
			// incase we only deleting the head enitty
			Entity entity{ ID,this };
			entity.EachChild([&](Entity childEntity) {
				childEntity.SetOwner(Entity{});
			});
			if (entity.HasOwner())
			{
				if (TryGetEntity(entity.GetOwner().GetEntityID()))
				{
					entity.GetOwner().RemoveChild(entity);
				}
			}

			for (auto&& pdata : m_Registry.pools)
			{
				pdata.pool&& pdata.pool->remove(ID.Get(), &m_Registry);
			}
			m_Registry.entities.erase(it);
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

	void World::OnRigidBodyComponentCreate(entt::registry64& component, uint64_t entityID)
	{
		m_PhysicsWorld->NewActor(entityID);
	}

	void World::OnRigidBodyComponentDelete(entt::registry64& component, uint64_t entityID)
	{
		m_PhysicsWorld->RemoveActor(entityID);
	}

	void World::OnScriptAdded(entt::registry64& component, uint64_t entityID)
	{
		ScriptMeathod::OnCreate({ entityID,this });
	}

	void World::OnScriptDelete(entt::registry64& component, uint64_t entityID)
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
							handler->Update(DeltaTime, GetWorldLocation(wfadfas));
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

	Entity World::TryGetEntity(UUID id) {
		auto it = std::find(m_Registry.entities.begin(), m_Registry.entities.end(), id.Get());
		if (it == m_Registry.entities.end())
			return Entity{};

		return Entity{ *it,this };
	}
	Entity World::GetEntity(UUID id)
	{
		return Entity{ id,this };
	}

	void World::OnSimulatePhysics(FrameTime DeltaTime) {
		DeleteEntitiesfromQeue();
	}

	bool World::HasWorldCamera() {
		const auto& cameraGroup = m_Registry.group<TransformComponent>(entt::get<CameraComponent>);
		return cameraGroup.size() > 0;
	}

	Entity World::GetWorldCameraEntity() {
		if (HasWorldCamera() == false)return Entity{ 0,nullptr };
		const auto& cameraGroup = m_Registry.group<TransformComponent>(entt::get<CameraComponent>);
		for (auto entity : cameraGroup)
		{
			return Entity{ entity,this };
		}
	}



	Entity World::CreateEntity(const std::string& EntName) {
		return CreateEntity(EntName, EntityID());
	}

	Entity World::CreateEntity(const std::string& EntName, EntityID ID) {
		/* we have to do some custmization of entt because when we pass an ID the entities create a vecot of the size of ID*/

		m_Registry.entities.emplace_back(ID.Get()); // not the correct way but it works there is some bugs with ent so we have to do this
		Entity entity = { ID,this };

		entity.AddComponent<IDComponent>(ID);
		entity.AddComponent<TagComponent>().Tag = EntName;
		entity.AddComponent<ChildComponent>().m_CurrentID = ID;
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	template<typename... Componnents>
	static void CopyComponentIfExistEntitySingle(Entity dst, Entity src)
	{
		([&]()
		{
			if (typeid(Componnents) == typeid(IDComponent) || typeid(Componnents) == typeid(ChildComponent))
			{
				return;
			}

			if (src.HasComponent<Componnents>())
				dst.AddorReplaceComponent<Componnents>(src.GetComponent<Componnents>());
		}(), ...);
	}
	template<typename... Component>
	static void CopyComponentIfExistsEntity(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExistEntitySingle<Component...>(dst, src);
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

	template<typename... Component>
	static void CopyComponentSinglPrefab(Entity dstEntity, entt::registry64& dstMap,const entt::registry64& src, UUID srcID,const std::unordered_map<UUID, uint64_t>& enttMap)
	{
		([&]()
		{
			if (typeid(Component) == typeid(IDComponent))
			{
				return;
			}

			uint64_t destinationID = dstEntity.GetEntityID();
			if (!src.any_of<Component>(srcID))
				return;

			const auto& srcComponent = src.get<Component>(srcID);
			dstMap.emplace_or_replace<Component>(destinationID, srcComponent);
			
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponentPrefab(ComponentGroup<Component...>, Entity dst, entt::registry64& dstMap,const entt::registry64& src,UUID srcID, const std::unordered_map<UUID, uint64_t>& enttMap)
	{
		CopyComponentSinglPrefab<Component...>(dst, dstMap, src, srcID, enttMap);
	}
	Entity World::CreateEntity(const std::string& name, Count<Prefab> prefab, TransformComponent transfom, EntityID id)
	{
		/**
		 * when an entity with sub children has rigid body this fucntions crahses 
		 * it could be a problem with the emplace or replace in the copy compoentnt single
		 * 
		 */
		Entity newEntity = CreateEntity(name, id);
		
		std::unordered_map<UUID, uint64_t> enttMap;
		{


			enttMap[prefab->GetBaseEntity()] = newEntity.GetEntityID();

			CopyComponentPrefab(AllComponents{}, newEntity, m_Registry, prefab->GetRegistry(), prefab->GetBaseEntity(), enttMap);
			newEntity.GetComponent<TransformComponent>() = transfom;


			newEntity.GetComponent<ChildComponent>().m_Children = {};
			newEntity.GetComponent<ChildComponent>().m_OwnerPointer = nullptr;
			newEntity.GetComponent<ChildComponent>().m_OwnerID = 0;
			newEntity.GetComponent<ChildComponent>().m_CurrentID = newEntity.GetEntityID();

			if (prefab->GetRegistry().size() == 1)
				return newEntity;
		}

		auto& prefaRegistry = prefab->GetRegistry();
		std::function<Entity(UUID)> createEntity = [&](UUID registryEntityId)->Entity
		{
			if (prefaRegistry.get<ChildComponent>(registryEntityId).HasOwner())
			{
				Entity owner;
				UUID owenrID = prefaRegistry.get<ChildComponent>(registryEntityId).GetOwnerID();
				if (!enttMap.contains(owenrID))
					owner = createEntity(owenrID);
				else
					owner = { enttMap[owenrID],this };

				Entity thisEntity = CreateEntity();
				enttMap[registryEntityId] = thisEntity.GetEntityID();
				CopyComponentPrefab(AllComponents{}, thisEntity, m_Registry, prefab->GetRegistry(), registryEntityId, enttMap);

				thisEntity.GetComponent<ChildComponent>().m_Children = {};
				thisEntity.GetComponent<ChildComponent>().m_OwnerPointer = nullptr;
				thisEntity.GetComponent<ChildComponent>().m_OwnerID = 0;
				thisEntity.GetComponent<ChildComponent>().m_CurrentID = thisEntity.GetEntityID();
				
			
				for (UUID id : prefaRegistry.get<ChildComponent>(registryEntityId).GetChildren())
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
					owner.GetComponent<ChildComponent>().AddChild(thisEntity.GetComponent<ChildComponent>());
				return thisEntity;
			}
			else
			{
				Entity basicEntity = CreateEntity();
				enttMap[registryEntityId] = basicEntity.GetEntityID();
				CopyComponentPrefab(AllComponents{}, basicEntity, m_Registry, prefab->GetRegistry(), registryEntityId, enttMap);

				return basicEntity;
			}
		};
		
		for (UUID registryEntityID : prefab->GetRegistry().entities)
		{
			if (registryEntityID == prefab->GetBaseEntity())continue;
			if (enttMap.contains(registryEntityID))
				continue;

			createEntity(registryEntityID);
		}

		return newEntity;
	}
	template<typename... Component>
	static void CopyComponentSingleWorld(entt::registry64& dst, entt::registry64& src, const std::unordered_map<UUID, uint64_t>& enttMap)
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
				uint64_t dstEntity = enttMap.at(id);

				auto& srcComponent = src.get<Component>(srcEntity);
				dst.emplace_or_replace<Component>(dstEntity, srcComponent);
			}
		}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry64& dst, entt::registry64& src, const std::unordered_map<UUID, uint64_t>& enttMap)
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
		std::unordered_map<UUID, uint64_t> enttMap;

		// Create entities in new scene
		// in reverse order
		auto idView = srcSceneRegistry.view<IDComponent>();
		std::for_each(idView.rbegin(), idView.rend(), [&](auto e) {
			EntityID uuid = srcSceneRegistry.get<IDComponent>(e).GetID();
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newWorld->CreateEntity(name, uuid);
			enttMap.insert({ uuid,newEntity.GetEntityID() });
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
				UUID playerID = entity.GetEntityID();
				DeleteEntity(entity);
				DeleteEntitiesfromQeue();
				return;
			}
			UUID playerID = entity.GetEntityID();
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
		config.Gravity = { 0,-500.8f,0 };// for multiplayer scene
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
		auto it = std::find(m_Registry.entities.begin(), m_Registry.entities.end(), ent.m_ID.Get());
		if (it == m_Registry.entities.end())
			return;
		m_EntityDeleteQueue.emplace_back(ent.GetEntityID());
		if (deleteChildren)
		{
			ent.EachChild([&](Entity childEntity) {
				DeleteEntity(childEntity, true);
			});
		}

	}

	Entity World::FindEntityByTag(const std::string& tag) {
		Entity returnEntity;
		ForEachEnitityWith<TagComponent>([&](Entity& entity) {
			if (entity.GetComponent<TagComponent>().Tag == tag)
			{
				returnEntity = entity;
				return;
			}

		});
		return returnEntity;
	}

	Vector World::GetWorldLocation(Entity entity) const {
		auto& transformComp = entity.GetComponent<TransformComponent>();
		if (entity.HasOwner())
			return transformComp.Location + World::GetWorldLocation(entity.GetOwner());
		return transformComp.Location;
	}

	Vector World::GetWorldRotation(Entity entity) const {
		auto& transformComp = entity.GetComponent<TransformComponent>();
		if (entity.HasOwner())
			return transformComp.Rotation + World::GetWorldRotation(entity.GetOwner());
		return transformComp.Rotation;
	}

	Vector World::GetWorldScale(Entity entity) const {
		auto& transformComp = entity.GetComponent<TransformComponent>();
		if (entity.HasOwner())
			return transformComp.Scale * World::GetWorldScale(entity.GetOwner());
		return transformComp.Scale;
	}

	TransformComponent World::GetWorldTransformComponent(Entity entity) const
	{
		TransformComponent transform;
		transform.Location = GetWorldLocation(entity);
		transform.Rotation = GetWorldRotation(entity);
		transform.Scale = GetWorldScale(entity);

		return transform;
	}
	
	glm::mat4 World::GetWorldTransform(Entity entity) const {
		auto rotation = GetWorldRotation(entity);
		return glm::translate(glm::mat4(1.0f), { ProofToglmVec(GetWorldLocation(entity)) }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.X), { 1,0,0 })
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.Y), { 0,1,0 })
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.Z), { 0,0,1 })
			* glm::scale(glm::mat4(1.0f), { ProofToglmVec(GetWorldScale(entity)) });
	}

	// need to fix this
	void World::OnChildComponentDestroy(ChildComponent& childComponent)
	{

	}
}