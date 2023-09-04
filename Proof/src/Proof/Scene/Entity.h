#pragma once
#include "World.h"
#include <type_traits>
#include "Proof/Core/Core.h"
#include "Proof/Scene/Component.h"
#include "glm/glm.hpp"

namespace Proof{
	struct TagComponent;
	struct TransformComponent;

	class Proof_API Entity {
	public:
		Entity(entt::entity handle, World* scene)
			: m_EntityHandle(handle), m_World(scene) {}
		Entity(const Entity& other) =default;
		Entity()=default;

		//template<class T>
		//T& GetComponent() {
		//	return CurrentWorld->m_Registry.get<T>(m_EntityHandle);
		//}
		template<class... Components>
		auto& GetComponent()
		{
			return m_World->m_Registry.get<Components...>(m_EntityHandle);
		}
		template<class... Components>
		auto& GetComponent()const
		{
			return m_World->m_Registry.get<Components...>(m_EntityHandle);
		}
		template<class... T>
		bool HasComponent()const {

			return m_World->m_Registry.all_of<T...>(m_EntityHandle);
		}
		template<class... T>
		bool HasAnyComponent()const {
			return m_World->m_Registry.any_of<T...>(m_EntityHandle);
		}
		template<class T,typename... Args>
		T& AddComponent(Args&&... args) {
			if (HasComponent<T>() == true) {
				PF_ERROR("Can not add component Entity already has {}", typeid(T).name());
				return GetComponent<T>();
			}

			T& Component = m_World->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			return Component;
		}
		template<typename T>
		bool RemoveComponent() {
			if (HasComponent<T>() == false)return false;
			m_World->m_Registry.remove<T>(m_EntityHandle);
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
		bool RemoveComponent<HierarchyComponent>() {
			PF_ERROR("cannot remove HierarchyComponent ");
			return false;
		}
		template<typename T, typename... Args>
		T& AddorReplaceComponent(Args&&... args) {// need to specify for child component and ID component
			T& Component = m_World->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			return Component;
		}
		template<typename T, typename... Args>
		T& GetorCreateComponent(Args&&... args) {
			T& Component = m_World->m_Registry.get_or_emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			return Component;
		}
		
		UUID GetParentUUID() {
			return GetComponent<HierarchyComponent>().ParentHandle;
		}
		Entity GetParent() {
			return m_World->TryGetEntityWithUUID(GetComponent<HierarchyComponent>().ParentHandle);
		}
		std::vector<UUID>& Children() { return GetComponent<HierarchyComponent>().Children; }
		const std::vector<UUID>& Children() const { return GetComponent<HierarchyComponent>().Children; }

		TransformComponent& Transform() { return m_World->m_Registry.get<TransformComponent>(m_EntityHandle); }
		const glm::mat4 Transform() const { return m_World->m_Registry.get<TransformComponent>(m_EntityHandle).GetTransform(); }
	
		bool HasChildren()
		{
			return !GetComponent<HierarchyComponent>().Children.empty();
		}
		void AddChild(Entity entity) {
			m_World->ParentEntity(entity, *this);
		}
		void SetParent(Entity parent) {
			m_World->ParentEntity(*this, parent);
		}
		void Unparent(bool convertoviewSPace = true)
		{
			m_World->UnparentEntity(*this, convertoviewSPace);
		}
		bool HasParent()
		{
			return m_World->HasEntity(GetComponent<HierarchyComponent>().ParentHandle);
		}
		void RemoveChild(Entity entity) {
			m_World->UnparentEntity(entity, true);
		}
		template<typename Func>
		void EachChild(Func func) {
			auto& children = Children();
			for (uint64_t ID = 0; ID < children.size();ID++) {
				Entity entity = m_World->GetEntity(children[ID]);
				func(entity);
			}
		}
		World* GetCurrentWorld()const {
			return m_World;
		}
		operator bool() const { return m_EntityHandle != entt::null && m_World != nullptr && m_World->m_Registry.valid(m_EntityHandle); }

		bool operator==(const Entity& other) const {
			return m_EntityHandle == other.m_EntityHandle && m_World == other.m_World;
		}

		bool operator!=(const Entity& other) const {
			return !(*this == other);
		}
		bool IsAncestorOf(Entity entity)
		{
			const auto& children = Children();

			if (children.empty())
			{
				return false;
			}

			for (UUID child : children)
			{
				if (child == entity.GetUUID())
				{
					return true;
				}
			}

			for (const UUID child : children)
			{
				if (m_World->GetEntity(child).IsAncestorOf(entity))
				{
					return true;
				}
			}

			return false;
		}

		bool IsDescendantOf(Entity entity) const
		{
			return entity.IsAncestorOf(*this);
		}


		std::string GetName();
		
		void SetName(const std::string& Name);
		//template<class... Component>
		//auto& GetComponent() {
		//	return CurrentWorld->m_Registry.get<Component...>(m_EntityHandle);
		//}

		// loops through all child entitues and check camera
		Entity GetCamera() {
			if (HasComponent< CameraComponent>())
				return *this;

			Entity cam{ entt::null,nullptr };
			EachChild([&](Entity child) {
				if (child.HasComponent<CameraComponent>())
				{
					cam = child;
					return;
				}
			});

			return cam;
		}
		UUID GetUUID() const 
		{ 
			if (!*this)
				return 0;
			return GetComponent<IDComponent>().GetID(); 
		}
		operator entt::entity() const { return m_EntityHandle; }
	private:
		
		World* m_World = nullptr;
		entt::entity m_EntityHandle{ entt::null };
		friend class World;
		friend class PhysicsEngine;
	};
}
