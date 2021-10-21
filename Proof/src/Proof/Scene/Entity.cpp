#include "Proofprch.h"
#include "Entity.h"
#include "Component.h"
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
}