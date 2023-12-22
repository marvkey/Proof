#pragma once
#include "Proof/Scene/Entity.h"
#include "Proof/Core/UUID.h"
#include "Proof/Asset/AssetTypes.h"
#include <unordered_map>
namespace Proof
{
	//https://github.com/Ant-Play/Ant/blob/2dab7c0362f017911df9090b1608ec4b81ad1f2c/Ant/src/Ant/Editor/SelectionManager.h
	enum class SelectionContext
	{
		Global = 0, Scene, ContentBrowser
	};

	class SelectionManager
	{
	public:
		static void Select(SelectionContext context, UUID selectionID);
		static bool IsSelected(UUID selectionID);
		static bool IsSelected(SelectionContext context, UUID selectionID);
		static bool IsEntityOrAncestorSelected(const Entity entity);
		static bool IsEntityOrAncestorSelected(SelectionContext context, const Entity entity);
		static void Deselect(UUID selectionID);
		static void Deselect(SelectionContext context, UUID selectionID);
		static void DeselectAll();
		static void DeselectAll(SelectionContext context);
		static UUID GetSelection(SelectionContext context, size_t index);

		static size_t GetSelectionCount(SelectionContext contextID);
		inline static const std::vector<UUID>& GetSelections(SelectionContext context) { return s_Contexts[context]; }

	private:
		inline static std::unordered_map<SelectionContext, std::vector<UUID>> s_Contexts;
	};

	enum class AssetSelectionContext
	{
		Prefab,GUIPanel
	};

	class AssetSelectionManager
	{
	public:
		static void Select(AssetSelectionContext context, AssetID assetID,UUID selectionID);
		static bool IsSelected(AssetSelectionContext context, AssetID assetID, UUID selectionID);
		static void Deselect(AssetSelectionContext context, AssetID assetID,UUID selectionID);
		static bool IsEntityOrAncestorSelected(AssetSelectionContext context, AssetID assetID, const Entity entity);
		static void DeselectAll(AssetSelectionContext context, AssetID assetID);
		static void DeselectAll(AssetSelectionContext context);
		static UUID GetSelection(AssetSelectionContext context, AssetID assetID,size_t index);
		static bool HasSelections(AssetSelectionContext contextID, AssetID assetID);
		static size_t GetSelectionCount(AssetSelectionContext contextID, AssetID assetID);
		inline static const std::vector<UUID>& GetSelections(AssetSelectionContext context, AssetID assetID) { return s_Contexts[context][assetID]; }

	private:
		//selction context, AssetID, SelectionIDs
		inline static std::unordered_map<AssetSelectionContext, std::unordered_map<UUID,std::vector<UUID>>> s_Contexts;

	};
}