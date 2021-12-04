#pragma once
#include "World.h"
#include <type_traits>
#include "Proof/Core/Core.h"
#include "Proof/Scene/Component.h"
namespace Proof{
	struct TagComponent;
	struct TransformComponent;
	struct SubEntityComponet;
	class Proof_API Entity {
	public:
		Entity(UUID ID,class World* world):
			m_ID(ID),CurrentWorld(world), m_EnttEntity(entt::entity((uint64_t)ID)) {}
		Entity(const Entity& other) =default;
		Entity()=default;

		template<class T>
		T* GetComponent() {
			if (HasComponent<T>() == false) return nullptr;
			return &CurrentWorld->m_Registry.get<T>(m_EnttEntity);
		}
		
		template<class T>
		bool HasComponent() {
			return CurrentWorld->m_Registry.any_of<T>(m_EnttEntity);
		}

		template<class T,typename... Args>
		T* AddComponent(Args&&... args) {
			if (HasComponent<T>() == true) {
				PF_WARN("Can not add component Entity already has component");
				return nullptr;
			}
			T* Component = &CurrentWorld->m_Registry.emplace<T>(m_EnttEntity, std::forward<Args>(args)...);
			CurrentWorld->OnComponentAdded<T>(this->GetID(), Component);
			return Component;
		}
		template<typename T>
		bool RemoveComponent() {
			if (HasComponent<T>() == false)return false;
			CurrentWorld->m_Registry.remove<T>(m_EnttEntity);
			return true;
		}
		template<>
		bool RemoveComponent<TagComponent>() {
			PF_ERROR("cannot remove component");
			return false;
		}
		template<>
		bool RemoveComponent<TransformComponent	>() {
			PF_ERROR("cannot remove Transform Component");
			return false;
		}
		template<>
		bool RemoveComponent<SubEntityComponet>() {
			PF_ERROR("cannot remove SubEntity Componet");
			return false;
		}
		World* GetCurrentWorld()const {
			return CurrentWorld;
		}
		operator bool() const { return m_ID != 0 &&CurrentWorld!=nullptr; }
		const UUID GetID()const { return m_ID;}

		bool operator==(const Entity& other) const {
			return m_ID == other.m_ID && CurrentWorld == other.CurrentWorld;
		}

		bool operator!=(const Entity& other) const {
			return !(*this == other);
		}

		std::string GetName();
		
		void SetName(const std::string& Name);
	private:
		
		World* CurrentWorld = nullptr;
		UUID m_ID{0};
		entt::entity m_EnttEntity{ entt::null };
		void OnSubEntityDelete(const Entity& tempEntity);
		void OnDelete();
		void SwapEntityOwner(const Entity& newOwner);
		friend class World;
	};
}
