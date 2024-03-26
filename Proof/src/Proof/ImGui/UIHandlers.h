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

	bool AttributeTextureAssetReference(const std::string& label, AssetID& id, const PropertyAssetReferenceSettings& settings = {});
	
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