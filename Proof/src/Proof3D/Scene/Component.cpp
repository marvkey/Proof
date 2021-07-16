#include "Proofprch.h"
#include "Component.h"
#include "Model.h"
#include "Entity.h"
namespace Proof{
	uint32_t MeshComponent::GetID() {
		return /* m_Asset!= nullptr&& m_Asset->m_Model != nullptr ? m_Asset->m_Model->GetID():*/ 0;
	}
	Entity Component::GetOwner() {
		return m_EntityOwner;
	}
}