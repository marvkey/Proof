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
		return GetMeshSource() != nullptr ? GetMeshSource()->GetID() : 0;
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
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, { GetWorldLocation() });
		model = glm::rotate(model, glm::radians(GetWorldRotation().X), { 1,0,0 });
		model = glm::rotate(model, glm::radians(GetWorldRotation().Y), { 0,1,0 });
		model = glm::rotate(model, glm::radians(GetWorldRotation().Z), { 0,0,1 });
		model = glm::scale(model, { GetWorldScale() });
		return model;
	}

	MeshAsset* MeshComponent::GetAsset() {
		/*
		if (m_MeshAssetPointerID == 0)
			return nullptr;
		if (m_MeshAssetPointer == nullptr)
			m_MeshAssetPointer = AssetManager::GetAssetShared<MeshAsset>(m_MeshAssetPointerID);

		if (m_MeshAssetPointer == nullptr)// if the last if statmetn make sthe mesh asset pointer still equal to null, no need to transverse again
			return nullptr;
		if (AssetManager::HasID(m_MeshAssetPointerID)) {
			return m_MeshAssetPointer.get();
		}
		m_MeshAssetPointerID = 0;
		m_MeshAssetPointer = nullptr;
		return nullptr;
		*/
		/*
		if (m_MeshAssetPointerID == 0)
			return nullptr;
		m_MeshAssetPointer = AssetManager::GetAssetShared<MeshAsset>(m_MeshAssetPointerID); // we reasign if the asset manager returns null then the
		// shared pointer is null

		if (m_MeshAssetPointer == nullptr) {// if the last if statmetn make sthe mesh asset pointer still equal to null, no need to transverse again
			m_MeshAssetPointerID = 0;
			return nullptr;
		}
		
		return m_MeshAssetPointer.get();
		*/
		/*
		if (m_MeshAssetPointerID == 0)
			return nullptr;
		if (AssetManager::HasID(m_MeshAssetPointerID)) {
			if (m_MeshAssetPointer == nullptr)
				m_MeshAssetPointer = AssetManager::GetAssetShared<MeshAsset>(m_MeshAssetPointerID);
			
			if(m_MeshAssetPointer->GetID() != m_MeshAssetPointerID)
				m_MeshAssetPointer = AssetManager::GetAssetShared<MeshAsset>(m_MeshAssetPointerID);

			return m_MeshAssetPointer.get();
		}
		m_MeshAssetPointerID = 0;
		return nullptr;
		*/
		if (m_MeshAssetPointerID == 0) {
			return nullptr;
		}
		if (AssetManager::HasID(m_MeshAssetPointerID)) {
			if (AssetManager::ForceGetAssetShared<MeshAsset>(m_MeshAssetPointerID) == nullptr) {
				AssetManager::NotifyOpenedNewAsset(m_MeshAssetPointerID);
				return nullptr;
			}
			m_MeshAssetPointer = AssetManager::ForceGetAssetShared<MeshAsset>(m_MeshAssetPointerID);
			return m_MeshAssetPointer.get();
		}

		m_MeshAssetPointer = nullptr;
		m_MeshAssetPointerID == 0;
		return nullptr;
	}
}
