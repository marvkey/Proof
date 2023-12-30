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
	enum class WorldState
	{
		Play,
		Pause,
		Simulate,
		Edit
	};

	class Proof_API World : public Asset{
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
		
		// using scripts
		virtual void StartRuntime();
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
		// entities get added to a que and deleted at teh e end of the frame
		void DeleteEntity(class Entity ent, bool deleteChildren = true);
		//only workdsd if it does not have a parent
		void ConvertToWorldSpaceTransform(Entity entity);

		glm::vec3 GetWorldSpaceLocation(Entity entity) const;
		glm::vec3 GetWorldSpaceRotation(Entity entity) const;
		glm::vec3 GetWorldSpaceScale(Entity entity) const;
		TransformComponent GetWorldSpaceTransformComponent(Entity entity) const;
		glm::mat4 GetWorldSpaceTransform(Entity entity) const;

		glm::mat4 GetWorldSpaceTransformUsingLocalRotation(Entity entity) const;
		TransformComponent GetWorldSpaceTransformComponentUsingLocalRotation(Entity entity) const;

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
	private:
		Camera m_Camera;
		glm::vec3 m_CameraPositon;
		// the parent entity shoudl be the prefab base entity 
		void PrefabCopyEntity(Count<class Prefab> prefab, Entity srcEntity, Entity parentEntity,bool includeChildren = true);

		void OnRender(Count<class WorldRenderer> renderer, FrameTime timestep,const Camera& camera, const glm::vec3& cameraLocation, float nearPlane, float farPlane);

		void RenderPhysicsDebug(Count<WorldRenderer> renderer, bool runtime);
		void RenderPhysicsDebug2D(Count<WorldRenderer> renderer, bool runtime);
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

		//https://github.com/Ant-Play/Ant/blob/2dab7c0362f017911df9090b1608ec4b81ad1f2c/Ant/src/Ant/Scene/Scene.h
		void BuildDynamicMeshEntityHierarchy(Entity parent, Count<DynamicMesh> mesh, const MeshNode& node, bool generateColliders);
		//void BuildBoneEntityIds(Entity entity);
		//void BuildMeshBoneEntityIds(Entity entity, Entity rootEntity);
		//void BuildAnimationBoneEntityIds(Entity entity, Entity rootEntity);

	private:
		std::unordered_set< UUID> m_EntityDeleteQueue;
		Count<class ScriptWorld> m_ScriptWorld;
		entt::registry m_Registry;
		Count<class PhysicsWorld> m_PhysicsWorld = nullptr;
		WorldState m_CurrentState = WorldState::Edit;
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
