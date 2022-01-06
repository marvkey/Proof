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
#include "Proof/Renderer/MeshWorkShop.h"
#include "Proof/Resources/Asset/AssetManager.h"
namespace Proof
{
	static struct Material Empty;
	
	UUID MeshComponent::GetMeshAssetID() {
		return GetMeshSource() != nullptr ? m_MeshAssetPointerID : 0;
	}
	Material* MeshComponent::GetMaterial() {
		/*
		if (m_MeshMaterialID == 0) {
			return nullptr;
		}
		MaterialAsset* a = AssetManager::GetAsset<MaterialAsset>(m_MeshMaterialID);
		if (a == nullptr) {
			m_MeshMaterialID = 0;
			return nullptr;
		}
		return &a->m_Material;
		*/
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
	void MeshComponent::SetMeshSource(UUID ID) {
		if (ID.Get() == 0)return;
		if (AssetManager::HasID(ID)) {
			m_MeshAssetPointer = AssetManager::ForceGetAssetShared<MeshAsset>(ID);
			m_MeshAssetPointerID = ID;
		}
	}	
	Vector<float> TransformComponent::GetWorldLocation()const {
		if (entID ==0 )return Location;
		Entity enttity(entID, m_World);
		if(enttity.HasOwner())
			return Location + enttity.GetOwner().GetComponent<TransformComponent>()->GetWorldLocation();
		return Location;
	}
	Vector<float> TransformComponent::GetWorldRotation()const {
		if (entID == 0)return Rotation;
		Entity enttity(entID, m_World);
		if (enttity.HasOwner())
			return Rotation + enttity.GetOwner().GetComponent<TransformComponent>()->GetWorldRotation();
		return Rotation;
	}
	Vector<float> TransformComponent::GetWorldScale()const {
		if (entID == 0)return Scale;
		Entity enttity(entID, m_World);
		if (enttity.HasOwner())
			return Scale + enttity.GetOwner().GetComponent<TransformComponent>()->GetWorldScale();
		return Scale;
	}
	glm::mat4 TransformComponent::GetWorldTransform() const {
		auto rotation = GetWorldRotation();
		return glm::translate(glm::mat4(1.0f), { GetWorldLocation()}) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.X), { 1,0,0 })
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.Y), { 0,1,0 })
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation.Z), { 0,0,1 })
			* glm::scale(glm::mat4(1.0f), { GetWorldScale()});
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
				return nullptr;
			}
			m_MeshAssetPointer = AssetManager::ForceGetAssetShared<MeshAsset>(m_MeshAssetPointerID);
			return m_MeshAssetPointer.get();
		}

		m_MeshAssetPointer = nullptr;
		m_MeshAssetPointerID == 0;
		return nullptr;
	}
	
	/*

		if (neweOwner == m_CurrentID || neweOwner == m_OwnerID) {
			PF_WARN("cannot add enity as owenr of entity");
			return false;
		}
		auto it = std::find(neweOwner.GetComponent<SubEntityComponet>()->m_AllSubEntity.begin(), neweOwner.GetComponent<SubEntityComponet>()->m_AllSubEntity.end(), neweOwner);
		if (it == neweOwner.GetComponent<SubEntityComponet>()->m_AllSubEntity.end()) {
			if (HasEntityOwner() == true) {
				m_Owner.GetComponent<SubEntityComponet>()->RemoveSubEnity(this->GetOwner());
			}
			m_Owner = neweOwner;
			neweOwner.GetComponent<SubEntityComponet>()->m_AllSubEntity.emplace_back(GetOwner());
		}
		*/
	
	Mesh* CubeColliderComponent::GetMeshSource(){
		return MeshWorkShop::GetCubeMesh().get();
	}

	Texture2DAsset* SpriteComponent::GetAsset()
	{
		if (m_TextureAssetPointerID == 0)
			return nullptr;
		if (m_TextureAssetPointer == nullptr)
			m_TextureAssetPointer = AssetManager::GetAssetShared<Texture2DAsset>(m_TextureAssetPointerID);

		if (m_TextureAssetPointer == nullptr)// if the last if statmetn make sthe mesh asset pointer still equal to null, no need to transverse again
			return nullptr;
		if (AssetManager::HasID(m_TextureAssetPointerID)) {
			return m_TextureAssetPointer.get();
		}
		m_TextureAssetPointerID = 0;
		m_TextureAssetPointer = nullptr;
		return nullptr;
	}

}
