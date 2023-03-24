#pragma once
#include "Proof/Core/Core.h"
#include "Camera/EditorCamera.h"
#include "Camera/OrthagraphicCamera.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/VertexArray.h"
#include "Proof/Renderer/FrameBuffer.h"
#include "Component.h"
#define ENTT_ID_TYPE uint64_t
#include "entt/entt.hpp"	
#include "entt/entity/group.hpp"
#include <tuple>
#include <variant>
#include <vector>

// cannot include prefab
class FrameTime;
namespace entt {
	using registry64 = basic_registry<uint64_t>;
};
namespace Proof {
	enum class WorldState
	{
		Play,
		Pause,
		Simulate,
		Edit
	};

	class Proof_API World {
	public:
		using EntityID = UUID;

		World(const std::string& name = "Default World", UUID ID = UUID());
		virtual ~World();

		AssetID GetID() {
			return m_WorldID;
		}
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
		bool HasEntity(EntityID ID)const;
		bool HasEntity(EntityID ID);
		bool HasWorldCamera();
		class Entity GetWorldCameraEntity();

		// using scripts
		virtual void StartRuntime();
		// using scripts
		virtual void EndRuntime();
		void OnUpdateRuntime(FrameTime DeltaTime);

		void OnUpdateEditor(FrameTime DeltaTime);
		void OnSimulatePhysics(FrameTime DeltaTime);

		Entity CreateEntity(const std::string& name, Count<class Prefab> prefab, Vector location);
		class Entity CreateEntity(const std::string& EntName = "Empty Entity");
		class Entity CreateEntity(const std::string& EntName, EntityID ID);
		class Entity CreateEntity(Entity entity, bool includeChildren = true);
		Entity TryGetEntity(UUID id);
		Entity GetEntity(UUID id);
		Entity FindEntityByTag(const std::string& tag);
		// entities get added to a que and deleted at teh e end of the frame
		void DeleteEntity(class Entity& ent, bool deleteChildren = true);

		Vector GetWorldLocation(Entity entity) const;
		Vector GetWorldRotation(Entity entity) const;
		Vector GetWorldScale(Entity entity) const;
		glm::mat4 GetWorldTransform(Entity entity) const;

		WorldState GetState() {
			return m_CurrentState;
		}
		template<class F>
		void ForEachEntity(F func) {
			for (uint64_t i = 0; i < m_Registry.entities.size(); i++)
			{
				Entity created{ m_Registry[i],this };
				func(created);
			}
		}
		template<class F>
		void ForEachEntityBackwards(F func) {
			for (auto it = m_Registry.entities.crbegin(); it != m_Registry.entities.crend(); ++it)
			{
				Entity created{ *it,this };
				func(created);
			}
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

		template <class ...T, class Func>
		void ForEachComponent(Func func) {
			auto view = m_Registry.view<T...>();
			view.pick_and_each(std::move(func), std::index_sequence_for<T...>{});
		}
		// returns number entities with componet
		// if more than one componenet passed it returns number entities with that combiniation of component
		template <class ...T>
		uint32_t GetNumComponents() {
			const auto& group = m_Registry.group<T>();
			return group.size();
		}

	private:
		void Init();
		void DeleteEntitiesfromQeue();
		
		// this is nore for beforee we strt runtime so we cna have mesh collider 
		// for debugging
		void OnMeshColliderComponentCreate(MeshColliderComponent& component);
		void OnMeshColliderComponentDelete(MeshColliderComponent& component);

		void OnChildComponentDestroy(ChildComponent& childComponent	);
		std::vector< EntityID> m_EntityDeleteQueue;
		entt::registry64 m_Registry;
		class PhysicsWorld* m_PhysicsWorld =nullptr;
		WorldState m_CurrentState = WorldState::Edit;
		UUID m_WorldID;
		std::string Name = "DefaultWorld";
		friend class SceneHierachyPanel;
		friend class Entity;
		friend class SceneSerializer;
		friend class Editore3D;
		friend class RendererBase;
		friend class WorldRenderer;
		friend class PrefabAssetSerilizer;
	};
}
