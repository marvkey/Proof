#include "Proofprch.h"
#include "AssetTypes.h"
namespace Proof::Utils
{
	std::string GetAssetExtensionString(AssetType type)
	{
		if (AssetTypeMap.contains(type))
			return AssetTypeMap.at(type);
		PF_CORE_ASSERT(false, "Unkown asset type");
		return "";
	}
}