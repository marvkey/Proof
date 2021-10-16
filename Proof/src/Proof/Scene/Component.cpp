#include "Proofprch.h"
#include "Component.h"
#include "Mesh.h"
#include "Entity.h"
#include "EntitiyComponentSystem/ECS.h"
#include "World.h"
#include "Proof/Resources/Asset/MaterialAsset.h"
#include "Material.h"
#include "script.h"
namespace Proof{
	static struct Material Empty;
	Entity Component::GetOwner() {
		return Entity{m_EntityOwner,CurrentWorld};
	}
	uint32_t MeshComponent::GetMeshPointerID() {
		return GetMesh()!= nullptr ? GetMesh()->GetID(): 0;
	}
	Material* MeshComponent::GetMaterial() {
		if(m_MeshMaterialID == 0){
			return nullptr;
		}
		MaterialAsset* a = AssetManager::GetAsset<MaterialAsset>(m_MeshMaterialID);
		if (a == nullptr){
			m_MeshMaterialID = 0;
			return nullptr;
		}
		return &a->m_Material;
	}
}