#pragma once
#include "Proof/Core/Core.h"
#include "Camera/Camera.h"
#include "Proof/Asset/Asset.h"
#include "Proof/Core/UUID.h"
#include "entt/entt.hpp"	
#include <tuple>
#include <variant>
#include <vector>

// cannot include prefab
class FrameTime;

namespace Proof {
	struct MeshColliderComponent;
	struct TransformComponent;
	struct DynamicMeshComponent;
	struct MeshNode;
	struct ElevatedInputKeyParams;



	enum class WorldState
	{
		Play,
		Pause,
		Simulate,
		Edit
	};

	struct RuntimeConfiguration
	{
		uint32_t PlayerInputCount = 1; // only the amount of players that can use input
		UUID WorldCameraEntity = 0;

		// TODO add unity type of multpile camera https://www.youtube.com/watch?v=rw2VKAdTdgQ&ab_channel=bendux
		// only applies to multiplayer
		enum class CameraSettings
		{
			PerPlayer,
			SingleCamera,
		};
		CameraSettings CameraOptions = CameraSettings::PerPlayer;
	};
	class Proof_API World : public Asset
	{
	public:
		using EntityID = UUID;

		World(const std::string& name = "Default World", UUID ID = UUID());
		virtual ~World();

		const std::string& GetName()const { return Name; };
		static Count<World> Copy(Count<World> worldToCopy);
		class Count<class PhysicsWorld> GetPhysicsWorld()const { return m_PhysicsWorld; };
		class Count<class ScriptWorld> GetScriptWorld()const { return m_ScriptWorld; };


		// if the scene is on play or pause 
		// does not use when is simualate
		bool IsPlaying() {
			return GetState() == WorldState::Play;
		}

		bool IsSimulating() {
			return GetState() == WorldState::Simulate;
		}

		void Pause() {
			m_CurrentState = WorldState::Pause;
		}
		void Play() {
			m_CurrentState = WorldState::Play;
		}
		bool HasEntity(UUID ID)const;
		bool HasWorldCamera();
		class Entity GetWorldCameraEntity();

		virtual void OnRenderEditor(Count<class WorldRenderer> renderer, FrameTime time, const class EditorCamera& camera);
		virtual void OnRenderRuntime(Count<class WorldRenderer> renderer, FrameTime time);
		void OnRender(Count<class WorldRenderer> renderer, FrameTime timestep, const Camera& camera, const glm::vec3& cameraLocation, float nearPlane, float farPlane, float Fov);

		// using scripts
		virtual void StartRuntime(RuntimeConfiguration runtimeConfig = RuntimeConfiguration());
		// using scripts
		virtual void EndRuntime();
		void OnUpdateRuntime(FrameTime DeltaTime);

		void OnUpdateEditor(FrameTime DeltaTime);
		void OnSimulatePhysics(FrameTime DeltaTime);

		Entity CreateEntity(const std::string& name, Count<class Prefab> prefab, TransformComponent transform, UUID id = UUID());
		class Entity CreateEntity(const std::string& EntName = "Empty Entity");
		class Entity CreateEntity(const std::string& EntName, EntityID ID);
		class Entity CreateEntity(Entity entity, bool includeChildren = true);
		Entity CreateEntity(Count<class DynamicMesh> mesh, bool generateCollider = true);
		Entity CreateChildEntity(Entity parent, const std::string& name);
		Entity TryGetEntityWithUUID(UUID id)const;
		Entity GetEntity(UUID id);
		Entity TryGetEntityByTag(const std::string& tag);
		Entity TryGetDescendantEntityWithTag(Entity entity, const std::string& tag);

		std::vector<UUID> FindBoneEntityIds(Entity entity, Entity rootEntity, Count<class Skeleton> skeleton);
		// entities get added to a que and deleted at teh e end of the frame
		void DeleteEntity(class Entity ent, bool deleteChildren = true, float time = 0.0f);
		//only workdsd if it does not have a parent
		void ConvertToWorldSpaceTransform(Entity entity);

		glm::vec3 GetWorldSpaceLocation(Entity entity) const;
		glm::vec3 GetWorldSpaceRotation(Entity entity) const;
		glm::vec3 GetWorldSpaceScale(Entity entity) const;
		TransformComponent GetWorldSpaceTransformComponent(Entity entity) const;
		glm::mat4 GetWorldSpaceTransform(Entity entity) const;

		glm::mat4 GetWorldSpaceTransformUsingLocalRotation(Entity entity) const;
		TransformComponent GetWorldSpaceTransformComponentUsingLocalRotation(Entity entity) const;
		std::vector<glm::mat4> GetModelSpaceBoneTransforms(const std::vector<UUID>& boneEntityIds, Count<class DynamicMesh> mesh);

		void ConvertToLocalSpace(Entity entity);
		void ParentEntity(Entity child, Entity parent);

		// checks if ansestor is an ansestor of entity
		void UnparentEntity(Entity entity, bool convertToWorldSpace = true);

		ASSET_CLASS_TYPE(World);

		WorldState GetState() {
			return m_CurrentState;
		}
		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}
		template<class ...T, class F>
		void ForEachEnitityWith(F func) {
			const auto& entitiyView = m_Registry.view<T...>();
			for (auto& entity : entitiyView)
			{
				Entity created{ entity,this };
				func(created);
			}
		}
		// returns number entities with componet
		// if more than one componenet passed it returns number entities with that combiniation of component
		template <class ...T>
		uint32_t GetNumComponents() {
			auto group = m_Registry.view<T...>();
			return group.size();
		}
		bool EnableRestart = false;

		const std::unordered_map<UUID, Entity>& GetEntities() { return m_EntitiesMap; };

		bool OnElevatedKeyEvent(const ElevatedInputKeyParams& keyParams);
		void OnWorldTransition(AssetID id);
		void SetWorldTransitionCallback(const std::function<void(AssetID)>& callback) { m_OnWorldTransitionCallback = callback; }
		Count<class DebugRenderer> GetDebugRenderer() { return m_DebugRenderer; }

		const RuntimeConfiguration& GetRuntimeConfig()const { return m_RuntimeConfig; }
		Count<class GameMode> GetGameMode()
		{
			return m_GameMode;
		}
		static std::vector<WeakCount<World>>&const GetAllActiveWorlds();

		void BuildBoneEntityIds(Entity entity);

	private:
		RuntimeConfiguration m_RuntimeConfig;

		Count<class DebugRenderer>  m_DebugRenderer;

		// the parent entity shoudl be the prefab base entity 
		void PrefabCopyEntity(Count<class Prefab> prefab, Entity srcEntity, Entity parentEntity,bool includeChildren = true);


		void RenderPhysicsDebug(Count<class WorldRenderer> renderer, bool runtime);
		void RenderPhysicsDebug2D(Count<class WorldRenderer> renderer, const Camera& camera, const glm::vec3& cameraLocation,bool runtime);
		void Init();
		void DeleteEntitiesfromQeue();
		
		// this is nore for beforee we strt runtime so we cna have mesh collider 
		// for debugging

		void OnRigidBodyComponentCreate(entt::registry& component, entt::entity entityID);
		void OnRigidBodyComponentDelete(entt::registry& component, entt::entity entityID);
		void OnMeshColliderComponentConstruct(entt::registry& registry, entt::entity entity);
		void OnMeshColliderComponentDestroy(entt::registry& registry, entt::entity entity);
		void OnScriptAdded(entt::registry& component, entt::entity entityID);
		void OnScriptDelete(entt::registry& component, entt::entity entityID);

		void OnUpdateAnimation(float deltaTime);

		void OnWaterComponentCreate(entt::registry& registry, entt::entity entity);
		//https://github.com/Ant-Play/Ant/blob/2dab7c0362f017911df9090b1608ec4b81ad1f2c/Ant/src/Ant/Scene/Scene.h
		void BuildDynamicMeshEntityHierarchy(Entity parent, Count<class DynamicMesh> mesh, const MeshNode& node, bool generateColliders);

		// same funciton just use the entity swap Ids
		// mainly for script components
		// first id is the dstEntity, second is srcEntity
		void PrefabCopyEntityReal(Count<class Prefab> prefab, Entity srcEntity, Entity parentEntity, std::unordered_map<UUID, UUID>& entitySwapID,bool includeChildren = true);
		// first id is the dstEntity, second is srcEntity
		class Entity CreateEntityFromOtherReal(Entity entity, std::unordered_map<UUID, UUID>& entitySwapID,bool includeChildren = true);

		void PauseRigidBodyOnConstruct()
		{
			m_RigidBodyOnConstruct = false;
		}
		void UnPauseRigidBodyOnConstruct();

		void BuildMeshBoneEntityIds(Entity entity, Entity rootEntity);
		void BuildAnimationBoneEntityIds(Entity entity, Entity rootEntity);
	private:

		std::vector<Entity> m_RigidBodyWaitingList;
		bool m_RigidBodyOnConstruct = true;
		std::function<void(AssetID)> m_OnWorldTransitionCallback;
		std::unordered_set< UUID> m_EntityDeleteQueue;
		std::unordered_map<UUID,float> m_EnttiesDeletAfterTime;
		Count<class ScriptWorld> m_ScriptWorld;
		entt::registry m_Registry;
		Count<class PhysicsWorld> m_PhysicsWorld = nullptr;
		WorldState m_CurrentState = WorldState::Edit;

		Count<class GameMode> m_GameMode;
		std::string Name = "DefaultWorld";
		std::unordered_map<UUID, Entity>m_EntitiesMap ;

		friend class SceneHierachyPanel;
		friend class Entity;
		friend class SceneSerializer;
		friend class Editore3D;
		friend class RendererBase;
		friend class WorldRenderer;
		friend class PrefabAssetSerilizer;
		friend class Prefab;
	};
}
