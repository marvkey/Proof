#include "Proofprch.h"
#include "AssetTypes.h"
#include "AssetManager.h"
#include "Asset.h"


#pragma region AssetsInclude
#include "Proof/Scene/Mesh.h"
#include "Proof/Physics/PhysicsMaterial.h"
#include "Proof/Physics/MeshCollider.h"
#include "Proof/Scene/Material.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Scene/Prefab.h"
#include "Proof/Input/ElevatedInputSystem/InputAction.h"
#include "Proof/Input/ElevatedInputSystem/InputBindingContext.h"
#include "Proof/Input/ElevatedInputSystem/InputCustomizers.h"

#pragma endregion
namespace Proof::Utils
{
	std::string GetAssetExtensionString(AssetType type)
	{
		if (AssetTypeMap.contains(type))
			return AssetTypeMap.at(type);
		PF_CORE_ASSERT(false, "Unkown asset type");
		return "";
	}
	bool HasAssetAndAssetType(AssetID id, AssetType type)
	{
		return AssetManager::HasAssetAndAssetType(id, type);
	}
	Count<Asset> GetAssetFromID(AssetID id)
	{
		return AssetManager::GetAsset<Asset>(id);
	}
	

	

}

namespace Proof
{

	StaticAssetKey::StaticAssetKey(const DynamicAssetKey& dynamicKey)
		: m_AssetID(0), m_ExpectedType(dynamicKey.GetExpectedType())
	{
		SetAssetID(dynamicKey.GetAssetID());
	}
}