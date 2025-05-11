#include "Proofprch.h"
#include "Component.h"
#include "Mesh.h"
#include "World.h"
#include "Proof/Asset/AssetManager.h"
#include "Material.h"
#include "script.h"
#include "Component.h"
#include "Mesh.h"
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
#include "Proof/Input/ElevatedInputSystem/ElevatedPlayer.h"
#include "Proof/Scene/WaterSystem/WaterSystem.h"
#include "Proof/Scene/WaterSystem/Water.h"
#include "Proof/Renderer/Font.h"
#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Renderer/ParticleSystem.h"
#include "Proof/Physics/MeshCollider.h"

#include "Material.h"

namespace Proof
{


	MeshComponent::MeshComponent()
	{
		MaterialTable = Count<class MaterialTable>::Create();

	}

	MeshComponent::MeshComponent(const MeshComponent& other)
		:
		m_MeshID(other.m_MeshID)
		, Visible(other.Visible)
		, CastShadow(other.CastShadow)
		, MaterialTable(Count<class MaterialTable>::CreateFrom(other.MaterialTable))
	{
	}
	SkyLightComponent::SkyLightComponent()
	{
		Environment = Count<class Environment>::Create();
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
	DynamicMeshComponent::DynamicMeshComponent() 
	{
		MaterialTable = Count<class MaterialTable>::Create();

	}
	DynamicMeshComponent::DynamicMeshComponent(const DynamicMeshComponent& other):
		m_MeshID(other.m_MeshID)
		,Visible(other.Visible)
		, CastShadow(other.CastShadow)
		,m_SubmeshIndex(other.m_SubmeshIndex)
		, MaterialTable(Count<class MaterialTable>::CreateFrom(other.MaterialTable))
	{
	}

	void DynamicMeshComponent::SetMesh(AssetID ID, bool copyMaterialTable)
	{
		//#ifdef PF_ENABLE_DEBUG
		if (!AssetManager::HasAssetAndAssetType(ID,AssetType::DynamicMesh))return;
		//#endif // 

		m_MeshID = ID;
		if (copyMaterialTable)
			MaterialTable = Count<class MaterialTable>::CreateFrom(AssetManager::GetAsset<DynamicMesh>(m_MeshID)->GetMaterialTable());

		SetSubMeshIndex(0);
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
		if (!AssetManager::HasAsset(m_MeshID)) 
		{ 
			m_MeshID = 0; 
			return nullptr;
		};
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
			m_SubmeshIndex = 0;
			return;
		}

		if (mesh->HasSubMesh(setSubMeshIndex))
			m_SubmeshIndex = setSubMeshIndex;

		MaterialTable = mesh->GetMaterialTableBasedOnSubMeshIndex(m_SubmeshIndex);
	}

	void SkyLightComponent::RemoveImage()
	{
		//Image = 0;
		//Environment = nullptr;
	}
	void SkyLightComponent::LoadMap(AssetID asset)
	{
		if (!AssetManager::HasAsset(asset))
			return;

		auto[irradiance, prefilter] = Renderer::CreateEnvironmentMap(AssetManager::GetAsset<Texture2D>(asset)->GetPath());

		//Environment = Count<class Environment>::Create(irradiance, prefilter);

		Image = asset;
	}

	PlayerInputComponent::PlayerInputComponent()
	{
		Player = Count<ElevatedPlayer>::Create();
	}

	PlayerInputComponent::PlayerInputComponent(Players playerInput, Count<class ElevatedPlayer> elevatedPlayer)
		:
		Player(elevatedPlayer)
	{
	}

	PlayerInputComponent::PlayerInputComponent(const PlayerInputComponent& other)
	{
		if (other.Player)
			Player = Count<ElevatedPlayer>::CreateFrom(other.Player);

	}

	ParticleSystemComponent::ParticleSystemComponent(const ParticleSystemComponent& other)
	{

		if (other.ParticleHandlerTable)
			ParticleHandlerTable = Count<class ParticleHandlerTable>::CreateFrom(other.ParticleHandlerTable);
	}

	PlayerHUDComponent::PlayerHUDComponent(const PlayerHUDComponent& other)
	{
		if (other.HudTable)
			HudTable = Count<UITable>::Create(other.HudTable);
	}

	PlayerHUDComponent::PlayerHUDComponent()
	{
		HudTable = Count<UITable>::Create();
	}
	WorldHUDComponent::WorldHUDComponent(const WorldHUDComponent& other)
	{
		if (other.HudTable)
			HudTable = Count<UITable>::Create(other.HudTable);
	}

	WorldHUDComponent::WorldHUDComponent()
	{
		HudTable = Count<UITable>::Create();

	}
	WaterComponent::WaterComponent()
	{
		Water = Count<class Water>::Create();
	}

	WaterComponent::WaterComponent(Count<class Water> water)
	{
		Water = water;
	}

	WaterComponent::WaterComponent(const WaterComponent& other)
	{
		Water = Count<class Water>::Create(other.Water);

	}

	WaterComponent::~WaterComponent()
	{
		Water = nullptr;
	}

	MeshColliderComponent::MeshColliderComponent(AssetID colliderID, uint32_t submeshIndex)
		: ColliderKey(colliderID), SubMeshIndex(submeshIndex)
	{
	}

}
