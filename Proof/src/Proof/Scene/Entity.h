#pragma once
#include "World.h"
#include <type_traits>
#include "Proof/Core/Core.h"
#include "Proof/Scene/Component.h"


namespace Proof{
	struct TagComponent;
	struct TransformComponent;

	using EntityID = UUID;
	class Proof_API Entity {
	public:
		Entity(EntityID ID,class World* world):
			m_ID(ID), CurrentWorld(world) {}
		Entity(const Entity& other) =default;
		Entity()=default;

		template<class... Component>
		auto GetComponent() {
			return CurrentWorld->m_Registry.try_get<Component...>(m_ID);
		}
		
		
		template<class... T>
		bool HasComponent()const {

			return CurrentWorld->m_Registry.any_of<T...>(m_ID.Get());
		}
		template<class... T>
		bool HasallComponent()const {
			return CurrentWorld->m_Registry.all_of<T...>(m_ID);
		}
		template<class T,typename... Args>
		T* AddComponent(Args&&... args) {
			if (HasComponent<T>() == true) {
				PF_ERROR("Can not add component Entity already has {}", typeid(T).name());
				return nullptr;
			}

			T* Component = &CurrentWorld->m_Registry.emplace<T>(m_ID, std::forward<Args>(args)...);
			return Component;
		}
		template<typename T>
		bool RemoveComponent() {
			if (HasComponent<T>() == false)return false;
			CurrentWorld->m_Registry.remove<T>(m_ID);
			return true;
		}
		template<>
		bool RemoveComponent<IDComponent>() {
			PF_ERROR("cannot remove ID Component");
			return false;
		}
		template<>
		bool RemoveComponent<TagComponent>() {
			PF_ERROR("cannot remove Tag Component");
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
		template<typename T, typename... Args>
		T* AddorReplaceComponent(Args&&... args) {// need to specify for child component and ID component
			T* Component = &CurrentWorld->m_Registry.emplace_or_replace<T>(m_ID, std::forward<Args>(args)...);
			return Component;
		}
		template<typename T, typename... Args>
		T* GetorCreateComponent(Args&&... args) {
			T* Component = &CurrentWorld->m_Registry.get_or_emplace<T>(m_ID, std::forward<Args>(args)...);
			return Component;
		}
		bool HasOwner() {
			return GetComponent<ChildComponent>()->HasOwner();
		}
		UUID GetOwnerUUID() {
			return GetComponent<ChildComponent>()->GetOwnerID();
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
				if(HasOwner())
					return GetOwner().RemoveChild(*this);
				return true;
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
		operator bool() const { return m_ID != 0 && CurrentWorld!=nullptr && CurrentWorld->HasEntity(m_ID); }
		const UUID GetEntityID()const { return m_ID;}

		bool operator==(const Entity& other) const {
			return m_ID == other.m_ID && CurrentWorld == other.CurrentWorld;
		}

		bool operator!=(const Entity& other) const {
			return !(*this == other);
		}

		std::string GetName();
		
		void SetName(const std::string& Name);
		template<class... Component>
		auto ForceGetComponent() {
			return CurrentWorld->m_Registry.get<Component...>(m_ID);
		}
	private:
		
		World* CurrentWorld = nullptr;
		EntityID m_ID{0};
		friend class World;
		friend class PhysicsEngine;
	};
}
