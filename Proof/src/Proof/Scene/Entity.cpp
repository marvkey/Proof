#include "Proofprch.h"
#include "Entity.h"
#include "Component.h"
namespace Proof
{
	std::string Entity::GetName(){
		return HasComponent<TagComponent>() != false ? GetComponent<TagComponent>().Tag : "UnnamedEntity";
	}
	void Entity::SetName(const std::string& Name) {
		if (HasComponent<TagComponent>())
			GetComponent<TagComponent>().Tag = Name;
	}
}