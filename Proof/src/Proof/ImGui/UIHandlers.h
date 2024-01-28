#pragma once
#include <imgui.h>
#include "UIColors.h"
#include "Proof/Asset/AssetTypes.h"
#include "UITypes.h"
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
	};

	bool AttributeAssetReference(const std::string& label, AssetType type, AssetID& id, const PropertyAssetReferenceSettings& settings = {});
	bool AttributeMultiAssetReference(const std::string& label,std::initializer_list< AssetType> assetTypes, AssetID& id, const PropertyAssetReferenceSettings& settings = {});

	bool AttributeTextureAssetReference(const std::string& label, AssetID& id, const PropertyAssetReferenceSettings& settings = {});

}