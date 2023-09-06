#pragma once
#include "Proof/Core/Core.h"
#include "Camera/EditorCamera.h"
#include "Proof/Asset/Asset.h"

#include "entt/entt.hpp"	
#include <tuple>
#include <variant>
#include <vector>

// cannot include prefab
class FrameTime;

namespace Proof {
	struct MeshColliderComponent;
	struct TransformComponent;

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
		class PhysicsWorld* GetPhysicsEngine()const { return m_PhysicsWorld; };


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

		virtual void OnRenderEditor(Count<class WorldRenderer> renderer, FrameTime time, const EditorCamera& camera);
		
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
	private:

		void OnRender(Count<class WorldRenderer> renderer, FrameTime timestep,const Camera& camera, const Vector& cameraLocation, float nearPlane, float farPlane);
		void Init();
		void DeleteEntitiesfromQeue();
		
		// this is nore for beforee we strt runtime so we cna have mesh collider 
		// for debugging
		void OnMeshColliderComponentCreate(MeshColliderComponent& component);
		void OnMeshColliderComponentDelete(MeshColliderComponent& component);

		void OnRigidBodyComponentCreate(entt::registry& component, entt::entity entityID);
		void OnRigidBodyComponentDelete(entt::registry& component, entt::entity entityID);
		void OnScriptAdded(entt::registry& component, entt::entity entityID);
		void OnScriptDelete(entt::registry& component, entt::entity entityID);

		std::unordered_set< UUID> m_EntityDeleteQueue;

		entt::registry m_Registry;
		class PhysicsWorld* m_PhysicsWorld =nullptr;
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
	};
}
