#pragma once
#include"entt/entt.hpp"
#include "World.h"
#include <type_traits>
namespace Proof{
	class Proof_API Entity {
	public:
		Entity::Entity(entt::entity Handle,World* _World,const std::string& _Name):
			EntityHandle(Handle),CurrentWorld(_World),Name(_Name) 	{	}
		Entity::Entity(entt::entity Handle,World* _World) :
			EntityHandle(Handle),CurrentWorld(_World) 	{	}
		Entity(const Entity& Other) = default;
		Entity() = default;
		template<class T>
		T* GetComponent() {
			if (HasComponent<T>() == false) return nullptr;
			return &CurrentWorld->Registry.get<T>(EntityHandle);
		}
		template<class T>
		bool HasComponent() {
			return CurrentWorld->Registry.has<T>(EntityHandle);
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
			T* Component = &CurrentWorld->Registry.emplace<T>(EntityHandle,std::forward<Args>(args)...);
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
		operator bool() const { return (bool)EntityHandle; };
		bool GetIsSpawned() {
			return IsSpawned;
		}
	private:
		World* CurrentWorld = nullptr;
		entt::entity EntityHandle{entt::null};
		std::string Name;
		friend class World;
		bool IsSpawned = false;
	};
}