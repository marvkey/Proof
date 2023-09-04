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
#include "Proof/Scene/Physics/PhysicsEngine.h"
#include "Proof/Scripting/ScriptEngine.h"
#include "Proof/Scene/Material.h"
#include "Proof/Renderer/Renderer.h"
namespace Proof
{

	Count<PhysicsMaterial> CubeColliderComponent::GetPhysicsMaterial(){
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
	}

	Count<Mesh> MeshComponent::GetMesh()
	{
		if (m_MeshID == 0)return nullptr;
		#ifdef PF_ENABLE_DEBUG
			if (!AssetManager::HasAsset(m_MeshID)) { m_MeshID = 0; return nullptr; };
		#endif 
		return AssetManager::GetAsset<Mesh>(m_MeshID);
	}
	

	Count<Mesh> MeshColliderComponent::GetMesh()
	{
		if (m_MeshAssetPointerID == 0)return nullptr;

		
		if (AssetManager::HasAsset(m_MeshAssetPointerID))
		{
			if (PhysicsMeshCooker::HasMesh(m_MeshAssetPointerID))
			{
				return PhysicsMeshCooker::GetConvexMeshAsMesh(m_MeshAssetPointerID);
			}
			return AssetManager::GetAsset<Mesh>(m_MeshAssetPointerID);
		}
		return nullptr;
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
