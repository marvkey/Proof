#include "Proofprch.h"
#include "Entity.h"
#include "Component.h"
namespace Proof
{
	std::string Entity::GetName() {
		return GetComponent<TagComponent>() != nullptr ? GetComponent<TagComponent>()->GetName() : "DefaultEntity";
	}
	void Entity::SetName(const std::string& Name) {
		auto* Tag = GetComponent<TagComponent>();
		if (Tag != nullptr)
			Tag->SetName(Name);
	}
	/*
	template<class T>
	T* Entity::GetComponent() {
		if (HasComponent<T>() == false) return nullptr;
		return CurrentWorld->Registry.GetComponent<T>(m_EntityID);
	}
	template<typename T>
	T* Entity::GetComponent(uint32_t ID,const std::string& CompName) {
		if (HasComponent<T>() == false) return nullptr;
		return CurrentWorld->Registry.GetComponent<T>(m_EntityID,CompName);
		PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
	}

	template<class T>
	bool Entity::HasComponent() {
		return CurrentWorld->Registry.HasComponent<T>(m_EntityID);
	}

	template<class T,typename ...Args>
	T* Entity::AddComponent(Args && ...args) {
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
		CurrentWorld->OnComponentAdded<T>(*this,Component);
		return Component;
	}

	template<typename T>
	void Entity::RemoveComponent() {
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
		CurrentWorld->Registry.RemoveComponent<T>(m_EntityID);
	}

	template<typename T>
	void Entity::RemoveComponent(uint64_t IndexSlot) {
		if (IndexSlot == 0) {
			PF_ENGINE_WARN("Cannot remove TagComponent");
			return;
		}

		if (IndexSlot == 1) {
			PF_ENGINE_WARN("Cannot remove Transform Component");
			return;
		}
		CurrentWorld->Registry.RemoveComponent<T>(m_EntityID,IndexSlot);
	}

	Entity::Entity(uint32_t EntityID,World* world):
		m_EntityID(EntityID),CurrentWorld(world)
	{
		
	}

	Component* Entity::GetComponent(uint32_t ID,uint32_t Index) {
		return CurrentWorld->Registry.GetComponent(m_EntityID,Index);
		PF_CORE_ASSERT(false,"Entity ID Was Not FOund");
	}

	World* Entity::GetCurrentWorld() {
		return CurrentWorld;
	}

	std::string Entity::GetName() {
		return GetComponent<TagComponent>() != nullptr ? GetComponent<TagComponent>()->GetName() : "DefaultEntity";
	}
	void Entity::SetName(const std::string& Name) {
		auto* Tag = GetComponent<TagComponent>();
		if (Tag != nullptr)
			Tag->SetName(Name);
	}
	*/
}