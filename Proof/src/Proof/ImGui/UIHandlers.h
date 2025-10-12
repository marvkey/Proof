#pragma once
#include "Proof/Core/Core.h"
#include <imgui.h>
#include "UIColors.h"
#include "Proof/Asset/AssetTypes.h"
#include "UITypes.h"
#include <vector>

namespace Proof
{
	class World;
	class MaterialTable;
	class UITable;
}
namespace Proof::UI
{
	struct PropertyAssetReferenceSettings
	{
		bool AdvanceToNextColumn = true;
		bool NoItemSpacing = false; // After label
		float WidthOffset = 0.0f;
		UIMemoryAssetTypes AssetMemoryTypes;
		ImVec4 ButtonLabelColor = ImGui::ColorConvertU32ToFloat4(Colours::Theme::Text);
		ImVec4 ButtonLabelColorError = ImGui::ColorConvertU32ToFloat4(Colours::Theme::TextError);
		bool ShowFullFilePath = false;
		std::function<void(AssetID& id)> OnRightClick = nullptr;
	};

	bool AttributeAssetReference(const std::string& label, AssetType type, AssetID& id, const PropertyAssetReferenceSettings& settings = {});
	bool AttributeMultiAssetReference(const std::string& label,std::initializer_list< AssetType> assetTypes, AssetID& id, const PropertyAssetReferenceSettings& settings = {});


	bool AttributeAssetReferenceList(const std::string& label, AssetType type, std::vector<AssetID>& id, const PropertyAssetReferenceSettings& settings = {});

	template <AssetType Type>
	bool AttributeAssetKeyReference(const std::string& label, AssetKey<Type>& assetKey, const PropertyAssetReferenceSettings& settings = {}) {
		AssetID id = assetKey.GetAssetID();
		if (AttributeAssetReference(label, Type, id, settings)) {
			assetKey.SetAssetID(id);
			return true;
		}
		return false;
	}

	template <AssetType Type>
	bool AttributeAssetKeysReferenceList(const std::string& label, std::vector<AssetKey<Type>>& assetKeys, const PropertyAssetReferenceSettings& settings = {}) 
	{

		std::vector<AssetID> ids;
		ids.resize(assetKeys.size());
		for (size_t i = 0; i < assetKeys.size(); ++i) {
			ids[i] = assetKeys[i].GetAssetID();
		}

		bool val = AttributeAssetReferenceList(label, Type, ids, settings);

		if (val) 
		{
			assetKeys.resize(ids.size());
			for (size_t i = 0; i < assetKeys.size(); ++i) {
				assetKeys[i].SetAssetID(ids[i]);
			}
		}
		
		return val;
	}
	bool AttributeAssetKeyReference(const std::string& label, DynamicAssetKey& assetKey, const PropertyAssetReferenceSettings& settings = {});
	bool AttributeAssetKeyReference(const std::string& label, StaticAssetKey& assetKey, const PropertyAssetReferenceSettings& settings = {});

	bool AttributeTextureAssetReference(const std::string& label, AssetID& id, const PropertyAssetReferenceSettings& settings = {});
	bool AttributeTextureAssetReference(const std::string& label, AssetKey<AssetType::Texture>& assetKey, const PropertyAssetReferenceSettings& settings = {});
	bool AttributeDrawMaterialTable(Count<MaterialTable> materialTable, Count<MaterialTable> sourceMaterialTable = nullptr);
	bool AttributeDrawUITable(Count<UITable> UITable);

	enum class AttributeVectorSupportedItemTypes
	{
		None = 0,
		Insert = BIT(0),
		Delete = BIT(1),
		Duplicate = BIT(2),

		All = Insert | Delete | Duplicate,
	};
	template<class Item>
	struct AttriuteVectorUsingDropDown
	{
		AttributeVectorSupportedItemTypes SettingsState = AttributeVectorSupportedItemTypes::All;

		std::function<std::pair<std::string,bool>(Item&)> ViewItem = nullptr; // name, is dropdown open

	};
	DEFINE_ENUM_CLASS_FLAGS(AttributeVectorSupportedItemTypes);

	
}