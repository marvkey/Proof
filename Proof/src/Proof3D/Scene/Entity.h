#pragma once
#include"entt/entt.hpp"
#include "World.h"
#include <type_traits>
namespace Proof{
	class Proof_API Entity {
	public:
		Entity::Entity(uint32_t EntityID,class World* world):
			m_EntityID(EntityID),CurrentWorld(world){}
		Entity(const Entity& Other) = default;
		Entity() = default;

		template<class T>
		T* GetComponent() {
			if (HasComponent<T>() == false) return nullptr;
			return CurrentWorld->Registry.GetComponent<T>(m_EntityID);
		}
		template<typename T>
		T* GetComponent(uint32_t ID,const std::string& CompName) {
			if (HasComponent<T>() == false) return nullptr;
			return CurrentWorld->Registry.GetComponent<T>(m_EntityID,CompName);
			PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
		}

		Component* GetComponent(uint32_t ID,uint32_t Index) {
			return CurrentWorld->Registry.GetComponent(m_EntityID,Index);
			PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
		}
		template<class T>
		bool HasComponent() {
			return CurrentWorld->Registry.HasComponent<T>(m_EntityID);
		}

		template<class T,typename... Args>
		T* AddComponent(Args&&... args) {
			bool HasTransfrom = std::is_same<T,TransformComponent>::value;
			if (HasTransfrom == true) {
				if (HasComponent<TransformComponent>() == true) {
					PF_ENGINE_WARN("Entity already has a transform ");
					return nullptr;
				}
			}
			bool  HasTag = std::is_same<T,TagComponent>::value;
			if (HasTag == true) {
				if (HasComponent<TagComponent>() == true) {
					PF_ENGINE_WARN("Entity already has a TagComponent");
					return nullptr;
				}
			}
			T* Component = CurrentWorld->Registry.AddComponent<T>(m_EntityID,std::forward<Args>(args)...);
			CurrentWorld->OnComponentAdded<T>(this,Component);
			return Component;
		}
		template<typename T>
		void RemoveComponent() {
			bool isHasTransfrom = std::is_same<T,TransformComponent>::value;
			if (isHasTransfrom == true){
				PF_ENGINE_WARN("Cannot remove Transform Component");
				return;
			}
			bool isHasTag = std::is_same<T,TagComponent>::value;
			if (isHasTag == true){
				PF_ENGINE_WARN("Cannot remove TagComponent");
				return;
			}
			if (HasComponent<T>() == false)return;
			CurrentWorld->Registry.RemoveComponent<T>(m_EntityID);
		}
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
			CurrentWorld->Registry.RemoveComponent(m_EntityID,Comp);
		}

		void RemoveComponent(uint64_t IndexSlot){
			if(IndexSlot ==0) {
				PF_ENGINE_WARN("Cannot remove TagComponent");
				return;
			}
			
			if(IndexSlot == 1){
				PF_ENGINE_WARN("Cannot remove Transform Component");
				return;
			}
			CurrentWorld->Registry.RemoveComponent(m_EntityID,IndexSlot);
		}
		World* GetCurrentWorld() {
			return CurrentWorld;
		}
		operator bool() const { return m_EntityID != 0; }
		uint32_t GetID() { return m_EntityID;}

		bool operator==(const Entity& other) const {
			return m_EntityID == other.m_EntityID /* && CurrentWorld == other.CurrentWorld*/;
		}

		bool operator!=(const Entity& other) const {
			return !(*this == other);
		}

		std::string GetName(){
			return GetComponent<TagComponent>()!=nullptr ? GetComponent<TagComponent>()->GetName() : "DefaultEntity";
		}
		
		void SetName(const std::string& Name){
			auto* Tag = GetComponent<TagComponent>();
			if(Tag!= nullptr)
				Tag->SetName(Name);
		}
	private:
		World* CurrentWorld = nullptr;
		uint32_t m_EntityID{0};
		friend class World;
	};
}