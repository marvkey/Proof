#include "Proofprch.h"
#include "Component.h"
#include "Mesh.h"
#include "Entity.h"
#include "EntitiyComponentSystem/ECS.h"
#include "World.h"
#include "Proof/Resources/Asset/MaterialAsset.h"
#include "Material.h"
#include "script.h"
#include "Component.h"
#include "Mesh.h"
#include "Entity.h"
#include "EntitiyComponentSystem/ECS.h"
#include "World.h"
#include "Proof/Resources/Asset/MaterialAsset.h"
#include "Material.h"
#include "script.h"
#include "Proof/Scene/ComponentUnOptimized.h"
namespace Proof
{
	static struct Material Empty;
	Entity Component::GetOwner()const {
		return Entity{m_EntityOwner,CurrentWorld};
	}
	uint32_t MeshComponent::GetMeshPointerID() {
		return GetMesh() != nullptr ? GetMesh()->GetID() : 0;
	}
	Material* MeshComponent::GetMaterial() {
		if (m_MeshMaterialID == 0) {
			return nullptr;
		}
		MaterialAsset* a = AssetManager::GetAsset<MaterialAsset>(m_MeshMaterialID);
		if (a == nullptr) {
			m_MeshMaterialID = 0;
			return nullptr;
		}
		return &a->m_Material;
	}
	Vector TransformComponent::GetWorldLocation()const {
		if(GetOwner().GetComponent<SubEntityComponet>()->HasEntityOwner()){
			return Location + GetOwner().GetComponent<SubEntityComponet>()->GetEntityOwner().GetComponent<TransformComponent>()->GetWorldLocation();
		}
		
		return Location;
	}
	Vector TransformComponent::GetWorldRotation()const {
		if (GetOwner().GetComponent<SubEntityComponet>()->HasEntityOwner()) {
			return Rotation + GetOwner().GetComponent<SubEntityComponet>()->GetEntityOwner().GetComponent<TransformComponent>()->GetWorldRotation();
		}
		return Rotation;
	}
	Vector TransformComponent::GetWorldScale()const {
		if (GetOwner().GetComponent<SubEntityComponet>()->HasEntityOwner()) {
			return Scale +GetOwner().GetComponent<SubEntityComponet>()->GetEntityOwner().GetComponent<TransformComponent>()->GetWorldScale();;
		}
		return Scale;
	}
	glm::mat4 TransformComponent::GetWorldTransform() const {
		glm::mat4 rotation = glm::toMat4(glm::quat(GetWorldRotation()));

		return glm::translate(glm::mat4(1.0f),{GetWorldLocation()})
			* rotation
			* glm::scale(glm::mat4(1.0f),{GetWorldScale()});
	}
}
