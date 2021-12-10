#pragma once
#include "World.h"
#include <type_traits>
#include "Proof/Core/Core.h"
#include "Proof/Scene/Component.h"
namespace Proof{
	struct TagComponent;
	struct TransformComponent;
	class Proof_API Entity {
	public:
		Entity(UUID ID,class World* world):
			m_ID(ID), CurrentWorld(world), m_EnttEntity(entt::entity(ID.Get())) {}
		Entity(const Entity& other) =default;
		Entity()=default;

		template<class T>
		T* GetComponent() {
			if (HasComponent<T>() == false) return nullptr;
			return &CurrentWorld->m_Registry.get<T>(m_ID);
		}
		
		template<class T>
		bool HasComponent() {
			return CurrentWorld->m_Registry.any_of<T>(m_ID);
		}

		template<class T,typename... Args>
		T* AddComponent(Args&&... args) {

			if (HasComponent<T>() == true) {
				PF_WARN("Can not add component Entity already has component");
				return nullptr;
			}

			T* Component = &CurrentWorld->m_Registry.emplace<T>(m_ID, std::forward<Args>(args)...);
			CurrentWorld->OnComponentAdded<T>(this->GetID(), Component);
			return Component;
		}
		template<typename T>
		bool RemoveComponent() {
			if (HasComponent<T>() == false)return false;
			CurrentWorld->m_Registry.remove<T>(m_ID);
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
		bool RemoveComponent<ChildComponent>() {
			PF_ERROR("cannot remove ChildComponent ");
			return false;
		}

		bool HasOwner() {
			return GetComponent<ChildComponent>()->HasOwner();
		}
		
		Entity GetOwner() {
			UUID id = GetComponent<ChildComponent>()->GetOwnerID();
			if (id == 0) return {};
			return Entity{ id ,CurrentWorld }; // if owner id is 0 entt still cant do anthing
		}
		bool RemoveChild(Entity entity) {
			if (!entity)return false;
			return GetComponent<ChildComponent>()->RemoveChild(*entity.GetComponent<ChildComponent>());
		}
		bool AddChild(Entity entity) {
			if (entity == Entity{})return false;
			return GetComponent<ChildComponent>()->AddChild(*entity.GetComponent<ChildComponent>());
		}
		bool SetOwner(Entity entity) {
			if (!entity) {

				return GetComponent<ChildComponent>()->SetOwnerEmpty();
			}
			return GetComponent<ChildComponent>()->SetOwner(*entity.GetComponent<ChildComponent>());
		}
		bool HasChild(Entity entity) {
			if (entity == Entity{})return false;
			return GetComponent<ChildComponent>()->HasChild(*entity.GetComponent<ChildComponent>());
		}

		bool HasChildren() {
			return GetComponent<ChildComponent>()->HasChildren();
		}
		template<typename Func>
		void EachChild(Func func) {
			ChildComponent* child = GetComponent<ChildComponent>();
			if (child == nullptr) return;
			for (uint64_t ID = 0; ID < child->m_Children.size();ID++) {
				Entity entity{ child->m_Children[ID],CurrentWorld};
				func(entity);
			}
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
		friend class World;
	};
}
