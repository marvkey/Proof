#pragma once
#include "World.h"
#include <type_traits>
#include "Proof/Core/Core.h"
namespace Proof{
	struct TagComponent;
	struct Component;
	struct TransformComponent;
	class Proof_API Entity {
	public:
		Entity(UUID UUID,class World* world):
			m_ID(UUID),CurrentWorld(world){}
		Entity(const Entity& Other) =default;
		Entity()=default;

		template<class T>
		T* GetComponent() {
			return CurrentWorld->Registry.GetComponent<T>(m_ID);
		}
		template<typename T>
		T* GetComponent(uint32_t ID,const std::string& CompName) {
			if (HasComponent<T>() == false) return nullptr;
			return CurrentWorld->Registry.GetComponent<T>(m_ID,CompName);
			PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
		}

		Component* GetComponent(uint32_t ID,uint32_t Index) {
			return CurrentWorld->Registry.GetComponent(m_ID,Index);
			PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
		}
		template<class T>
		bool HasComponent() {
			return CurrentWorld->Registry.HasComponent<T>(m_ID);
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
			/*
			bool  HasSubEntityComp = std::is_same<T,SubEntityComponet>::value;
			if (HasSubEntityComp == true) {
				if (HasComponent<SubEntityComponet>() == true) {
					PF_ENGINE_WARN("Entity already has a SubEntityComponet");
					return nullptr;
				}
			}
			*/
			T* Component = CurrentWorld->Registry.AddComponent<T>(m_ID,std::forward<Args>(args)...);
			CurrentWorld->OnComponentAdded<T>(*this,Component);
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
			/*
			bool  HasSubEntityComp = std::is_same<T,SubEntityComponet>::value;
			if (HasSubEntityComp == true) {
				PF_ENGINE_WARN("Entity already has a SubEntityComponet");
				return nullptr;
			}
			*/
			if (HasComponent<T>() == false)return;
			CurrentWorld->Registry.RemoveComponent<T>(m_ID);
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
			CurrentWorld->Registry.RemoveComponent<T>(m_ID,IndexSlot);
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
