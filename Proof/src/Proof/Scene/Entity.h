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
		Entity(UUID UUID,class World* world):
			m_ID(UUID),CurrentWorld(world){}
		Entity(const Entity& other) =default;
		Entity()=default;

		template<class T>
		T* GetComponent() {
			return &CurrentWorld->m_Registry.get<T>(entt::entity(m_ID.Get()));
		}
		
		template<class T>
		bool HasComponent() {
			return CurrentWorld->m_Registry.has<T>(entt::entity(m_ID.Get()));
		}

		template<class T,typename... Args>
		T* AddComponent(Args&&... args) {
			bool HasTransfrom = std::is_same<T,TransformComponent>::value;
			if (HasTransfrom == true) {
				if (HasComponent<TransformComponent>() == true) {
					PF_WARN("Entity already has a transform");
					return nullptr;
				}
			}
			bool  HasTag = std::is_same<T,TagComponent>::value;
			if (HasTag == true) {
				if (HasComponent<TagComponent>() == true) {
					PF_WARN("Entity already has a TagComponent");
					return nullptr;
				}
			}
			T* Component = &CurrentWorld->m_Registry.emplace<T>(entt::entity(m_ID.Get()), std::forward<Args>(args)...);
			CurrentWorld->OnComponentAdded<T>(this, *Component);
			return Component;
		}
		template<typename T>
		void RemoveComponent() {
			bool isHasTransfrom = std::is_same<T,TransformComponent>::value;
			if (isHasTransfrom == true){
				PF_WARN("Cannot remove Transform Component");
				return;
			}
			bool isHasTag = std::is_same<T,TagComponent>::value;
			if (isHasTag == true){
				PF_WARN("Cannot remove TagComponent");
				return;
			}
			/*
			bool  HasSubEntityComp = std::is_same<T,SubEntityComponet>::value;
			if (HasSubEntityComp == true) {
				PF_ENGINE_WARN("Entity already has a SubEntityComponet");
				return nullptr;
			}
			*/
			if (HasComponent<T>() == false)return;
			CurrentWorld->m_Registry.remove<T>(m_ID);
		}
		/*
		template<typename T>
		void RemoveComponent(Component* Comp){
			bool isHasTransfrom = std::is_same<T,TransformComponent>::value;
			if (isHasTransfrom == true) {
				PF_ENGINE_WARN("Cannot remove Transform Component");
				return;
			}
			bool isHasTag = std::is_same<T,TagComponent>::value;
			if (isHasTag == true) {
				PF_ENGINE_WARN("Cannot remove TagComponent");
				return;
			}
			if (HasComponent<T>() == false)return;
			CurrentWorld->Registry.RemoveComponent(m_ID,Comp);
		}
		*/
		template<typename T>
		void RemoveComponent(uint64_t IndexSlot){
			if(IndexSlot ==0) {
				PF_ENGINE_WARN("Cannot remove TagComponent");
				return;
			}
			
			if(IndexSlot == 1){
				PF_ENGINE_WARN("Cannot remove Transform Component");
				return;
			}
			CurrentWorld->m_Registry.remove<T>(m_ID,IndexSlot);
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
		void OnSubEntityDelete(const Entity& tempEntity);
		void OnDelete();
		void SwapEntityOwner(const Entity& newOwner);
		friend class World;
	};
}
