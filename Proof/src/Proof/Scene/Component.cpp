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
	Vector<float> TransformComponent::GetWorldLocation()const {
		/*
		if(GetOwner().GetComponent<SubEntityComponet>()->HasEntityOwner()){
			return Location + GetOwner().GetComponent<SubEntityComponet>()->GetEntityOwner().GetComponent<TransformComponent>()->GetWorldLocation();
		}
		*/
		return Location;
		
	}
	Vector<float> TransformComponent::GetWorldRotation()const {
		/*
		if (GetOwner().GetComponent<SubEntityComponet>()->HasEntityOwner()) {
			return Rotation + GetOwner().GetComponent<SubEntityComponet>()->GetEntityOwner().GetComponent<TransformComponent>()->GetWorldRotation();
		}
		*/
		return Rotation;
		
	}
	Vector<float> TransformComponent::GetWorldScale()const {
		/*
		if (GetOwner().GetComponent<SubEntityComponet>()->HasEntityOwner()) {
			return Scale +GetOwner().GetComponent<SubEntityComponet>()->GetEntityOwner().GetComponent<TransformComponent>()->GetWorldScale();;
		}
		*/
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
	Count<Mesh> CubeColliderComponent::m_CubeMesh = MeshWorkShop::GetCubeMesh();
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
	bool ChildComponent::SetOwner(UUID newOwner){
		if (m_CurrentWorld == nullptr)return false;
		Entity entity = {newOwner,m_CurrentWorld };
		Entity currentEntity{ m_CurrentID, m_CurrentWorld };
		Entity ownerEntity{ m_OwnerID, m_CurrentWorld };
		if (newOwner == m_CurrentID || newOwner == m_OwnerID) {
			PF_WARN("cannot add enity as owenr of entity");
			return false;
		}
		auto it = std::find(entity.GetComponent<ChildComponent>()->m_AllSubEntity.begin(), entity.GetComponent<ChildComponent>()->m_AllSubEntity.end(), newOwner);
		if (it == entity.GetComponent<ChildComponent>()->m_AllSubEntity.end()) {
			if (HasOwner() == true) {
				ownerEntity.GetComponent<ChildComponent>()->RemoveChild(m_CurrentID);
			}
			m_OwnerID = newOwner;
			ownerEntity.GetComponent<ChildComponent>()->m_AllSubEntity.emplace_back(m_CurrentID);
			return true;
		}
		return false;
	}

	bool ChildComponent::AddChild(UUID child) {
		if (m_CurrentWorld == nullptr)return false;
		Entity childentity = { child,m_CurrentWorld };
		if (child == m_CurrentID || child == m_OwnerID) {
			PF_WARN("cannot add enity as owenr of entity");
			return false;
		}
		auto it = std::find(m_AllSubEntity.begin(), m_AllSubEntity.end(), child);
		if (it == m_AllSubEntity.end()) {
			m_AllSubEntity.emplace_back(child);
			childentity.GetComponent<ChildComponent>()->m_OwnerID = m_CurrentID;
			return true;
		}
		return false;
	}

	bool ChildComponent::RemoveChild(UUID child) {
		if (m_AllSubEntity.size() == 0)
			return false;
		if (m_CurrentWorld == nullptr)return false;
		Entity childentity = { child,m_CurrentWorld };
		auto it = std::find(m_AllSubEntity.begin(), m_AllSubEntity.end(), child);
		if (it != m_AllSubEntity.end()) {
			m_AllSubEntity.erase(it);
			childentity.GetComponent<ChildComponent>()->m_OwnerID = 0;
			return true;
		}
		return false;
	}
}
