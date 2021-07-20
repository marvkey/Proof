#include "Proofprch.h"
#include "Component.h"
#include "Model.h"
#include "Entity.h"
#include "EntitiyComponentSystem/ECS.h"
#include "World.h"
namespace Proof{
	Entity Component::GetOwner() {
		return Entity{m_EntityOwner,CurrentWorld};
	}
	uint32_t MeshComponent::GetMeshPointerID() {
		return GetModel()!= nullptr ? GetModel()->GetID(): 0;
	}
}