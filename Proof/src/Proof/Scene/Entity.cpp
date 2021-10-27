#include "Proofprch.h"
#include "Entity.h"
#include "Component.h"
#include "ComponentUnOptimized.h"
namespace Proof
{
	std::string Entity::GetName(){
		return GetComponent<TagComponent>() != nullptr ? GetComponent<TagComponent>()->GetName() : "DefaultEntity";
	}
	void Entity::SetName(const std::string& Name) {
		auto* Tag = GetComponent<TagComponent>();
		if (Tag != nullptr)
			Tag->SetName(Name);
	}

	void Entity::OnSubEntityDelete(const Entity& tempEntity) {// Called when subbed entity is deleted
		GetComponent<SubEntityComponet>()->RemoveSubEnity(tempEntity);
	}
	void Entity::OnDelete() {
		for (Entity& m_SubEntity : GetComponent<SubEntityComponet>()->m_AllSubEntity) {
			CurrentWorld->DeleteEntity(m_SubEntity);
		}
		
		if (GetComponent<SubEntityComponet>()->HasEntityOwner() == true){
			GetComponent<SubEntityComponet>()->GetEntityOwner().GetComponent<SubEntityComponet>()->RemoveSubEnity({m_EntityID,CurrentWorld});
		}
		
	};

	void Entity::SwapEntityOwner(const Entity& newOwner) {
		GetComponent<SubEntityComponet>()->GetEntityOwner().OnSubEntityDelete({m_EntityID,CurrentWorld});
		GetComponent<SubEntityComponet>()->m_EntitySubOwner = newOwner;
	}
}