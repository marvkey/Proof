#pragma once
#include"entt/entt.hpp"
#include "World.h"
#include <type_traits>
namespace Proof
{
	class Proof_API Entity {
	public:
		Entity::Entity(entt::entity Handle,World* world,const std::string& name):
			m_EntityHandle(Handle),CurrentWorld(world),Name(name) {}
		Entity::Entity(entt::entity Handle,World* _World) :
			m_EntityHandle(Handle),CurrentWorld(_World) {}
		Entity(const Entity& Other) = default;
		Entity() = default;

		template<class T>
		T* GetComponent() {
			if (HasComponent<T>() == false) return nullptr;
			return &CurrentWorld->Registry.get<T>(m_EntityHandle);
		}
		template<class T>
		bool HasComponent() {
			return CurrentWorld->Registry.has<T>(m_EntityHandle);
		}

		template<class T,typename... Args>
		T* AddComponent(Args&&... args) {
			bool  HasTransfrom = std::is_same<T,TransformComponent>::value;
			if (HasTransfrom == true) {
				if (HasComponent<TransformComponent>() == true) {
					PF_ENGINE_INFO("Entity already has a transform ");
					return nullptr;
				}
			}
			bool  HasTag = std::is_same<T,TagComponent>::value;
			if (HasTag == true) {
				if (HasComponent<TagComponent>() == true) {
					PF_ENGINE_INFO("Entity already has a TagComponent");
					return nullptr;
				}
			}
			T* Component = &CurrentWorld->Registry.emplace<T>(m_EntityHandle,std::forward<Args>(args)...);
			CurrentWorld->OnComponentAdded<T>(this,*Component);
			return Component;
		}
		template<typename T>
		void RemoveComponent() {
			if (HasComponent<T>() == false)return;
			CurrentWorld->Registry.remove<T>(EntityHandle);
		}
		World* GetCurrentWorld() {
			return CurrentWorld;
		}
		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		uint32_t GetID() { return (uint32_t)m_EntityHandle; }
		/*Temporary */
		bool operator==(const Entity& other) const {
			return m_EntityHandle == other.m_EntityHandle && CurrentWorld == other.CurrentWorld;
			//return *this ==other;																																																																																																																																																																																																																																																																																																																																																																																																																																																											
		}

		bool operator!=(const Entity& other) const {
			return !(*this == other);
		}
		bool GetIsSpawned() {
			return IsSpawned;
		}

		const std::string& GetName() { return Name; }
		std::string Name = "Entity";
	private:
		World* CurrentWorld = nullptr;

		entt::entity m_EntityHandle{entt::null};
		friend class World;
		bool IsSpawned = false;
	};
}