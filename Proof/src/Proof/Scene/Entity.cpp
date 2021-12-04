#include "Proofprch.h"
#include "Entity.h"
#include "Component.h"
namespace Proof
{
	std::string Entity::GetName(){
		return GetComponent<TagComponent>() != nullptr ? GetComponent<TagComponent>()->Tag : "DefaultEntity";
	}
	void Entity::SetName(const std::string& Name) {
		auto* Tag = GetComponent<TagComponent>();
		if (Tag != nullptr)
			Tag->Tag = Name;
	}

	void Entity::OnSubEntityDelete(const Entity& tempEntity) {// Called when subbed entity is deleted
		//GetComponent<SubEntityComponet>()->RemoveSubEnity(tempEntity);
	}
	void Entity::OnDelete() {
		/*
		for (Entity m_SubEntity : GetComponent<SubEntityComponet>()->m_AllSubEntity) {
			if(m_SubEntity != *this){
				CurrentWorld->m_Registry.remove(entt::entity((uint64_t)m_SubEntity.GetID()));
			}
		}
	*/
		/*
		if (GetComponent<SubEntityComponet>()->HasEntityOwner() == true) {
			GetComponent<SubEntityComponet>()->GetEntityOwner().GetComponent<SubEntityComponet>()->RemoveSubEnity(*this);
		}
		*/
	};

	void Entity::SwapEntityOwner(const Entity& newOwner) {
		/*
		GetComponent<SubEntityComponet>()->GetEntityOwner().OnSubEntityDelete({m_ID,CurrentWorld});
		GetComponent<SubEntityComponet>()->m_EntitySubOwner = newOwner;
		*/
	}
}