#include "Proofprch.h"
#include "World.h"
#include "Component.h"
#include "Entity.h"
#include "Script.h"
#include "Proof/Renderer/RendererBase.h"
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
namespace Proof {
	World::World(const std::string& name, UUID ID)
		:
		m_WorldID(ID), Name(name)
	{
		Init();
	}
	World::~World()
	{
		//m_Registry.on_construct<MeshColliderComponent>().disconnect<&World::OnMeshColliderComponentCreate>(this);
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
	}
	void World::Init()
	{
		//m_Registry.on_construct<MeshColliderComponent>().connect<&World::OnMeshColliderComponentCreate>(this);
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
		{
			const auto& scriptView = m_Registry.view<ScriptComponent>();
			for (auto entity : scriptView)
			{
				auto& script = scriptView.get<ScriptComponent>(entity);
				ScriptEngine::OnUpdate(DeltaTime, Entity{ entity,this });
			}
		}
		m_PhysicsWorld->OnUpdate(DeltaTime);

		DeleteEntitiesfromQeue();
	}

	Entity World::TryGetEntity(UUID id) {
		auto it = std::find(m_Registry.entities.begin(), m_Registry.entities.end(), id.Get());
		if (it == m_Registry.entities.end())
			return Entity{};

		return Entity{ *it,this };
	}
	template<typename Component>
	static Component* CopyComponentIfExists(Entity dst, Entity src)
	{
		if (src.HasComponent<Component>())
			return dst.AddorReplaceComponent<Component>(*src.GetComponent<Component>());
		return nullptr;
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
		entity.AddComponent<TagComponent>()->Tag = EntName;
		entity.AddComponent<ChildComponent>()->m_CurrentID = ID;
		entity.AddComponent<TransformComponent>();
		return entity;
	}
	Entity World::CreateEntity(Entity entity, bool includeChildren) {
		Entity newEntity = CreateEntity(entity.GetName());
		CopyComponentIfExists<TagComponent>(newEntity, entity);
		CopyComponentIfExists<TransformComponent>(newEntity, entity);
		//CopyComponentIfExists<ChildComponent>(Entity{ ID,newWorld.get() }, Entity{ ID,other.get() });
		CopyComponentIfExists<NativeScriptComponent>(newEntity, entity);
		CopyComponentIfExists<MeshComponent>(newEntity, entity);
		CopyComponentIfExists<CameraComponent>(newEntity, entity);

		CopyComponentIfExists<CubeColliderComponent>(newEntity, entity);
		CopyComponentIfExists<SphereColliderComponent>(newEntity, entity);
		CopyComponentIfExists<CapsuleColliderComponent>(newEntity, entity);
		CopyComponentIfExists<MeshColliderComponent>(newEntity, entity);
		CopyComponentIfExists<RigidBodyComponent>(newEntity, entity);

		CopyComponentIfExists<ScriptComponent>(newEntity, entity);

		CopyComponentIfExists<DirectionalLightComponent>(newEntity, entity);
		CopyComponentIfExists<PointLightComponent>(newEntity, entity);
		CopyComponentIfExists<SpotLightComponent>(newEntity, entity);
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
		//newWorld->m_WorldID = other->m_WorldID;
	
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

	void World::EndRuntime() {
		ScriptEngine::EndWorld();
		delete m_PhysicsWorld;
	}

	void World::StartRuntime() {

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
				ScriptEngine::StartWorld(this);
				auto view = m_Registry.view<ScriptComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					ScriptEngine::OnCreate(entity);
					ScriptEngine::OnPlace(entity);
				}
			}
		}
		m_PhysicsWorld = new PhysicsWorld(this, PhysicsWorldConfig());
	}

	void World::DeleteEntity(Entity& ent, bool deleteChildren) {
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
		ForEachEnitityWith<TagComponent>([&](Entity& entity) {
			if (entity.GetComponent<TagComponent>()->Tag == tag)
				return entity;
		});
		return { 0, nullptr };
	}

	Vector World::GetWorldLocation(Entity entity) const {
		auto& transformComp = *entity.GetComponent<TransformComponent>();
		if (entity.HasOwner())
			return transformComp.Location + World::GetWorldLocation(entity.GetOwner());
		return transformComp.Location;
	}

	Vector World::GetWorldRotation(Entity entity) const {
		auto& transformComp = *entity.GetComponent<TransformComponent>();
		if (entity.HasOwner())
			return transformComp.Rotation + World::GetWorldRotation(entity.GetOwner());
		return transformComp.Rotation;
	}

	Vector World::GetWorldScale(Entity entity) const {
		auto& transformComp = *entity.GetComponent<TransformComponent>();
		if (entity.HasOwner())
			return transformComp.Scale + World::GetWorldScale(entity.GetOwner());
		return transformComp.Scale;
	}

	glm::mat4 World::GetWorldTransform(Entity entity) const {
		auto rotation = GetWorldRotation(entity);
		return glm::translate(glm::mat4(1.0f), { ProofToglmVec(GetWorldLocation(entity)) }) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.X), { 1,0,0 })
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.Y), { 0,1,0 })
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.Z), { 0,0,1 })
			* glm::scale(glm::mat4(1.0f), { ProofToglmVec(GetWorldScale(entity)) });
	}
}