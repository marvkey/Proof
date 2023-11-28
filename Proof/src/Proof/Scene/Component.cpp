#include "Proofprch.h"
#include "Component.h"
#include "Mesh.h"
#include "Entity.h"
#include "EntitiyComponentSystem/ECS.h"
#include "World.h"
#include "Proof/Asset/AssetManager.h"
#include "Material.h"
#include "script.h"
#include "Component.h"
#include "Mesh.h"
#include "Entity.h"
#include "EntitiyComponentSystem/ECS.h"
#include "World.h"
#include "Material.h"
#include "script.h"
#include "Proof/Renderer/MeshWorkShop.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Physics/PhysicsEngine.h"
#include "Proof/Scripting/ScriptEngine.h"
#include "Proof/Scene/Material.h"
#include "Proof/Physics/PhysicsMaterial.h"
#include "Proof/Renderer/Renderer.h"
namespace Proof
{

	Count<PhysicsMaterial> BoxColliderComponent::GetPhysicsMaterial(){
		if (m_PhysicsMaterialPointerID == 0)
		{
			return nullptr;
		}
		if (AssetManager::HasAsset(m_PhysicsMaterialPointerID))
		{
			return AssetManager::GetAsset<PhysicsMaterial>(m_PhysicsMaterialPointerID);
		}
		m_PhysicsMaterialPointerID = 0;
		return nullptr;
	}
	Count<PhysicsMaterial> SphereColliderComponent::GetPhysicsMaterial(){
		if (m_PhysicsMaterialPointerID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<PhysicsMaterial>(m_PhysicsMaterialPointerID);
		if (a == nullptr) {
			m_PhysicsMaterialPointerID = 0;
			return nullptr;
		}
		return a;
	}

	Count<PhysicsMaterial> CapsuleColliderComponent::GetPhysicsMaterial(){
		if (m_PhysicsMaterialPointerID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<PhysicsMaterial>(m_PhysicsMaterialPointerID);
		if (a == nullptr)
		{
			m_PhysicsMaterialPointerID = 0;
			return nullptr;
		}
		return a;
	}
	Count<PhysicsMaterial> MeshColliderComponent::GetPhysicsMaterial() {
		if (m_PhysicsMaterialPointerID == 0) {
			return nullptr;
		}
		auto a = AssetManager::GetAsset<PhysicsMaterial>(m_PhysicsMaterialPointerID);
		if (a == nullptr)
		{
			m_PhysicsMaterialPointerID = 0;
			return nullptr;
		}
		return a;
	}

	void MeshComponent::SetMesh(UUID ID, bool copyMaterialTable )
	{
		#ifdef PF_ENABLE_DEBUG
			if (!AssetManager::HasAsset(ID))return;
		#endif // 

		m_MeshID = ID;
		if (copyMaterialTable)
			MaterialTable =Count<class MaterialTable>::CreateFrom( AssetManager::GetAsset<Mesh>(m_MeshID)->GetMaterialTable());
	}

	void MeshComponent::RemoveMesh()
	{
		m_MeshID = 0;
		MaterialTable = Count<class MaterialTable>::Create();
	}

	Count<Mesh> MeshComponent::GetMesh()
	{
		if (m_MeshID == 0)return nullptr;
	//	#ifdef PF_ENABLE_DEBUG
			if (!AssetManager::HasAsset(m_MeshID)) { m_MeshID = 0; return nullptr; };
		//#endif 
		return AssetManager::GetAsset<Mesh>(m_MeshID);
	}
	


	void DynamicMeshComponent::SetMesh(AssetID ID, bool copyMaterialTable)
	{
		//#ifdef PF_ENABLE_DEBUG
		if (!AssetManager::HasAsset(ID))return;
		//#endif // 

		m_MeshID = ID;
		if (copyMaterialTable)
			MaterialTable = Count<class MaterialTable>::CreateFrom(AssetManager::GetAsset<DynamicMesh>(m_MeshID)->GetMaterialTable());
	}
	void DynamicMeshComponent::RemoveMesh()
	{
		m_MeshID = 0;
		m_SubmeshIndex = 0;
		MaterialTable = Count<class MaterialTable>::Create();
	}
	uint32_t DynamicMeshComponent::GetSubMeshMaterialIndex()
	{
		auto mesh =GetMesh();
		if (mesh)
		{
			auto& subMesh = mesh->GetMeshSource()->GetSubMeshes().at(m_SubmeshIndex);
			return subMesh.MaterialIndex;
		}
		return 0;
	}
	Count<class DynamicMesh> DynamicMeshComponent::GetMesh()
	{
		if (m_MeshID == 0)return nullptr;
//	#ifdef PF_ENABLE_DEBUG
		if (!AssetManager::HasAsset(m_MeshID)) { m_MeshID = 0; return nullptr; };
	//#endif 
		return AssetManager::GetAsset<DynamicMesh>(m_MeshID);
	}
	uint32_t DynamicMeshComponent::GetSubMeshIndex()
	{

		return m_SubmeshIndex;
	}
	void DynamicMeshComponent::SetSubMeshIndex(uint32_t setSubMeshIndex)
	{
		auto mesh = GetMesh();
		if (mesh == nullptr)
		{
			m_SubmeshIndex = setSubMeshIndex;
			return;
		}

		if (mesh->HasSubMesh(setSubMeshIndex))
			m_SubmeshIndex = setSubMeshIndex;
	}

	void SkyLightComponent::RemoveImage()
	{
		Image = 0;
		Environment = nullptr;
	}
	void SkyLightComponent::LoadMap(AssetID asset)
	{
		if (!AssetManager::HasAsset(asset))
			return;

		auto[irradiance, prefilter] = Renderer::CreateEnvironmentMap(AssetManager::GetAsset<Texture2D>(asset)->GetPath());

		Environment = Count<class Environment>::Create(irradiance, prefilter);

		Image = asset;
	}

}
