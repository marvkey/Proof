#include "Proofprch.h"
#include "UIHandlers.h"
#include "UI.h"
#include "Proof/Asset/AssetManager.h"
#include "UiUtilities.h"
#include "UIWidgets.h"
#include "Proof/Scene/Material.h"
#include "Proof/Renderer/RenderMaterial.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Platform/Vulkan/VulkanTexutre.h"
#include "Proof/Renderer/Renderer.h"
#include "Editors/EditorResources.h"
#include "Proof/Project/Project.h"
#include "Proof/Renderer/UIRenderer/UIMenu.h"
#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Utils/ContainerUtils.h"
#include "UIVariable.h"

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
namespace Proof::UI
{
    bool AttributeAssetReference(const std::string& label, AssetType assetType, AssetID& outHandle, const PropertyAssetReferenceSettings& settings)
    {
#if 0
		bool modified = false;

        UI::ShiftCursor(10.0f, 9.0f);
        UI::AttributeLabel(label);
        ImGui::NextColumn();
        UI::ShiftCursorY(4.0f);

        ImGui::PushItemWidth(-1);

		ImVec2 originalButtonTextAlign = ImGui::GetStyle().ButtonTextAlign;
		{
			ImGui::GetStyle().ButtonTextAlign = { 0.0f, 0.5f };
			float width = ImGui::GetContentRegionAvail().x - settings.WidthOffset;
			float itemHeight = 28.0f;

			std::string buttonText = fmt::format("Null ({})",EnumReflection::EnumString(assetType));
			bool valid = true;
			if (AssetManager::HasAsset(outHandle))
			{
				const auto& assetInfo = AssetManager::GetAssetInfo(outHandle);
				if (settings.ShowFullFilePath)
					buttonText = assetInfo.Path.string();
				else
					buttonText = assetInfo.GetName();
			}

			if ((GImGui->CurrentItemFlags & ImGuiItemFlags_MixedValue) != 0)
				buttonText = "---";

			// PropertyAssetReference could be called multiple times in same "context"
			// and so we need a unique id for the asset search popup each time.
			// notes
			// - don't use GenerateID(), that's inviting id clashes, which would be super confusing.
			// - don't store return from GenerateLabelId in a const char* here. Because its pointing to an internal
			//   buffer which may get overwritten by the time you want to use it later on.
			std::string assetSearchPopupID = UI::GenerateLabelID("ARSP");
			{
				UI::ScopedStyleColor buttonLabelColor(ImGuiCol_Text, valid ? settings.ButtonLabelColor : settings.ButtonLabelColorError);
				ImGui::Button(GenerateLabelID(buttonText), { width, itemHeight });

				const bool isHovered = ImGui::IsItemHovered();

				if (isHovered)
				{
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						//AssetEditorPanelInterface::OpenEditor(AssetManager::GetAsset<Asset>(outHandle));
					}
					else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						ImGui::OpenPopup(assetSearchPopupID.c_str());
					}
				}
			}

			ImGui::GetStyle().ButtonTextAlign = originalButtonTextAlign;

			bool clear = false;
			if (Widgets::AssetSearchPopup(assetSearchPopupID.c_str(), assetType, outHandle, settings.AssetMemoryTypes))
			{
				modified = true;
			}
		}

		if (!IsItemDisabled())
		{
			if (ImGui::BeginDragDropTarget())
			{
				auto data = ImGui::AcceptDragDropPayload("asset_payload");

				if (data)
				{
					AssetID assetHandle = *(AssetID*)data->Data;
					if (AssetManager::HasAsset(assetHandle))
					{
						const auto& assetInfo = AssetManager::GetAssetInfo(assetHandle);
						if (assetInfo.Type == assetType)
						{
							outHandle = assetHandle;
							modified = true;
						}
					}
				}
			}
		}

		if (!IsItemDisabled())
			DrawItemActivityOutline(2.0f, true, Colours::Theme::Accent);

		ImGui::PopItemWidth();
		if (settings.AdvanceToNextColumn)
		{
			ImGui::NextColumn();
			Draw::Underline();
		}

		return modified;
#endif

		return AttributeMultiAssetReference(label, { assetType }, outHandle, settings);

    }
	bool AttributeMultiAssetReference(const std::string& label, std::initializer_list<AssetType> assetTypesList, AssetID& outHandle, const PropertyAssetReferenceSettings& settings)
	{
		bool modified = false;

		UI::ShiftCursor(0.f, 9.0f);
		UI::AttributeLabel(label);
		ImGui::NextColumn();
		UI::ShiftCursorY(4.0f);

		ImGui::PushItemWidth(-1);

		std::unordered_set<AssetType> assetTypesSet = assetTypesList;

		ImVec2 originalButtonTextAlign = ImGui::GetStyle().ButtonTextAlign;
		{
			ImGui::GetStyle().ButtonTextAlign = { 0.0f, 0.5f };
			float width = ImGui::GetContentRegionAvail().x - settings.WidthOffset;
			float itemHeight = 28.0f;

			std::string buttonText = "Null";

			for (auto& assetType : assetTypesSet)
			{
				buttonText += fmt::format(" ({})", EnumReflection::EnumString(assetType));
			}
			bool valid = true;
			if (AssetManager::HasAsset(outHandle))
			{
				const auto& assetInfo = AssetManager::GetAssetInfo(outHandle);
				if (settings.ShowFullFilePath)
					buttonText = assetInfo.Path.string();
				else
					buttonText = assetInfo.GetName();
			}

			if ((GImGui->CurrentItemFlags & ImGuiItemFlags_MixedValue) != 0)
				buttonText = "---";

			// PropertyAssetReference could be called multiple times in same "context"
			// and so we need a unique id for the asset search popup each time.
			// notes
			// - don't use GenerateID(), that's inviting id clashes, which would be super confusing.
			// - don't store return from GenerateLabelId in a const char* here. Because its pointing to an internal
			//   buffer which may get overwritten by the time you want to use it later on.
			std::string assetSearchPopupID = UI::GenerateLabelID("ARSP");
			{
				UI::ScopedStyleColor buttonLabelColor(ImGuiCol_Text, valid ? settings.ButtonLabelColor : settings.ButtonLabelColorError);
				ImGui::Button(GenerateLabelID(buttonText), { width, itemHeight });

				const bool isHovered = ImGui::IsItemHovered();
				{
					UI::ScopedStyleColor popupBG(ImGuiCol_PopupBg, UI::ColourWithMultipliedValue(Colours::Theme::Background, 1.6f).Value);

					if (ImGui::BeginPopup((assetSearchPopupID + "RemovePopUp").c_str()))
					{
						if (ImGui::MenuItem("Remove"))
						{
							outHandle = 0;
							modified = true;

						}

						if (settings.OnRightClick)
							settings.OnRightClick(outHandle);
						ImGui::EndPopup();
					}
				}
				if (isHovered)
				{
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						//AssetEditorPanelInterface::OpenEditor(AssetManager::GetAsset<Asset>(outHandle));
					}
					else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						ImGui::OpenPopup(assetSearchPopupID.c_str());
					}
					else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
					{
						ImGui::OpenPopup((assetSearchPopupID + "RemovePopUp").c_str());
					}
					
				}
			}

			ImGui::GetStyle().ButtonTextAlign = originalButtonTextAlign;

			bool clear = false;
			if (Widgets::AssetSearchPopup(assetSearchPopupID.c_str(), assetTypesList, outHandle, settings.AssetMemoryTypes))
			{
				modified = true;
			}
		}

		if (!IsItemDisabled())
		{
			if (ImGui::BeginDragDropTarget())
			{
				auto data = ImGui::AcceptDragDropPayload("asset_payload");

				if (data)
				{
					AssetID assetHandle = *(AssetID*)data->Data;
					if (AssetManager::HasAsset(assetHandle))
					{
						const auto& assetInfo = AssetManager::GetAssetInfo(assetHandle);
						for (auto& assetType : assetTypesSet)
						{
							if (assetInfo.Type == assetType)
							{
								outHandle = assetHandle;
								modified = true;
							}
						}
					}
				}
			}
		}

		if (!IsItemDisabled())
			DrawItemActivityOutline(2.0f, true, Colours::Theme::Accent);

		ImGui::PopItemWidth();
		if (settings.AdvanceToNextColumn)
		{
			ImGui::NextColumn();
			Draw::Underline();
		}
		HandleModified(modified);
		return modified;
	}
	bool AttributeAssetReferenceList(const std::string& label, AssetType type, std::vector<AssetID>& assetIDs, const PropertyAssetReferenceSettings& settings)
	{
		bool modified = false;

		if (ImGui::Button("+", ImVec2{ ImGui::GetContentRegionAvail().x,20 }))
		{
			assetIDs.push_back(AssetID());
			modified = true;
		}


		for (uint32_t assetIndex = 0; assetIndex < assetIDs.size(); assetIndex++)
		{
			std::string layerLabel = fmt::format("[{} Index: {}]", label, assetIndex);
			std::string layerId = fmt::format("{0}-{1}", layerLabel, assetIndex);

			ImGui::PushID(layerId.c_str());

			AssetID& ID = assetIDs[assetIndex];
			if (UI::AttributeAssetReference(layerLabel, type, ID))
			{
				modified = true;
			}


			if (ImGui::Button("Remove", ImVec2{ ImGui::GetContentRegionAvail().x,20 }))
			{
				assetIDs.erase(assetIDs.begin() + assetIndex);
				modified = true;
			}
			ImGui::PopID();

		}
		UI::HandleModified(modified);
		return modified;
	}
	bool AttributeAssetKeyReference(const std::string& label, DynamicAssetKey& assetKey, const PropertyAssetReferenceSettings& settings)
	{
		AssetID id = assetKey.GetAssetID();
		if (AttributeAssetReference(label, assetKey.GetExpectedType(), id, settings)) {
			assetKey.SetAssetID(id);
			return true;
		}
		return false;
	}
	bool AttributeAssetKeyReference(const std::string& label, StaticAssetKey& assetKey, const PropertyAssetReferenceSettings& settings)
	{
		AssetID id = assetKey.GetAssetID();
		if (AttributeAssetReference(label, assetKey.GetExpectedType(), id, settings)) {
			assetKey.SetAssetID(id);
			return true;
		}
		return false;
	}
	bool AttributeTextureAssetReference(const std::string& label, AssetID& outHandle, const PropertyAssetReferenceSettings& settings)
	{
		bool modified = false;

		UI::ShiftCursor(0.f, 9.0f);
		UI::AttributeLabel(label);
		ImGui::NextColumn();
		UI::ShiftCursorY(4.0f);

		ImGui::PushItemWidth(-1);

		auto assetTypesList = { AssetType::Texture };
		std::unordered_set<AssetType> assetTypesSet = assetTypesList;

		ImVec2 originalButtonTextAlign = ImGui::GetStyle().ButtonTextAlign;
		{
			ImGui::GetStyle().ButtonTextAlign = { 0.0f, 0.5f };
			//float width = ImGui::GetContentRegionAvail().x - settings.WidthOffset;
			float width = 100.0f;
			//float itemHeight = 28.0f;
			float itemHeight = 100.0f;

			std::string buttonText = "Null";

			for (auto& assetType : assetTypesSet)
			{
				buttonText += fmt::format(" ({})", EnumReflection::EnumString(assetType));
			}
			bool valid = true;
			if (AssetManager::HasAsset(outHandle))
			{
				const auto& assetInfo = AssetManager::GetAssetInfo(outHandle);
				if (settings.ShowFullFilePath)
					buttonText = assetInfo.Path.string();
				else
					buttonText = assetInfo.GetName();
			}

			if ((GImGui->CurrentItemFlags & ImGuiItemFlags_MixedValue) != 0)
				buttonText = "---";

			// PropertyAssetReference could be called multiple times in same "context"
			// and so we need a unique id for the asset search popup each time.
			// notes
			// - don't use GenerateID(), that's inviting id clashes, which would be super confusing.
			// - don't store return from GenerateLabelId in a const char* here. Because its pointing to an internal
			//   buffer which may get overwritten by the time you want to use it later on.
			std::string assetSearchPopupID = UI::GenerateLabelID("ARSP");
			{
				UI::ScopedStyleColor buttonLabelColor(ImGuiCol_Text, valid ? settings.ButtonLabelColor : settings.ButtonLabelColorError);
				//ImGui::Button(GenerateLabelID(buttonText), { width, itemHeight });

				//UI::AttributeLabel(GenerateLabelID(buttonText));
				//ImGui::SameLine();
				auto texture = AssetManager::HasAsset(outHandle) && AssetManager::IsAssetLoaded(outHandle) ? AssetManager::GetAsset<Texture2D>(outHandle) : EditorResources::CheckerBoardWhiteGrey;
				UI::ImageButton(texture, { width,itemHeight });
				const bool isHovered = ImGui::IsItemHovered();
				{
					UI::ScopedStyleColor popupBG(ImGuiCol_PopupBg, UI::ColourWithMultipliedValue(Colours::Theme::Background, 1.6f).Value);

					if (ImGui::BeginPopup((assetSearchPopupID + "RemovePopUp").c_str()))
					{
						if (ImGui::MenuItem("Remove"))
						{
							outHandle = 0;
							modified = true;

						}
						ImGui::EndPopup();
					}
				}
				if (isHovered)
				{
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						//AssetEditorPanelInterface::OpenEditor(AssetManager::GetAsset<Asset>(outHandle));
					}
					else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
					{
						ImGui::OpenPopup(assetSearchPopupID.c_str());
					}
					else if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
					{
						ImGui::OpenPopup((assetSearchPopupID + "RemovePopUp").c_str());
					}

				}
			}

			ImGui::GetStyle().ButtonTextAlign = originalButtonTextAlign;

			bool clear = false;
			if (Widgets::AssetSearchPopup(assetSearchPopupID.c_str(), assetTypesList, outHandle, settings.AssetMemoryTypes))
			{
				modified = true;
			}
		}

		if (!IsItemDisabled())
		{
			if (ImGui::BeginDragDropTarget())
			{
				auto data = ImGui::AcceptDragDropPayload("asset_payload");

				if (data)
				{
					AssetID assetHandle = *(AssetID*)data->Data;
					if (AssetManager::HasAsset(assetHandle))
					{
						const auto& assetInfo = AssetManager::GetAssetInfo(assetHandle);
						for (auto& assetType : assetTypesSet)
						{
							if (assetInfo.Type == assetType)
							{
								outHandle = assetHandle;
								modified = true;
							}
						}
					}
				}
			}
		}

		if (!IsItemDisabled())
			DrawItemActivityOutline(2.0f, true, Colours::Theme::Accent);

		ImGui::PopItemWidth();
		if (settings.AdvanceToNextColumn)
		{
			ImGui::NextColumn();
			Draw::Underline();
		}

		HandleModified(modified);
		return modified;
	}

	bool AttributeTextureAssetReference(const std::string& label, AssetKey<AssetType::Texture>& assetKey, const PropertyAssetReferenceSettings& settings)
	{
		AssetID id = assetKey.GetAssetID();

		if(AttributeTextureAssetReference(label, id, settings))
		{
			assetKey.SetAssetID(id);
			return true;
		}
		return false;
	}

	struct ConvertRuntimeMaterialToDiskMaterial
	{
		std::string SavePath = "Materials/";

	}ConvertDiskMaterial;

	static Count<Texture2D> MaterialCreateTextures(const std::filesystem::path& directory, Count<Texture2D> texture)
	{
		if (AssetManager::HasAsset(texture))
		{
			if (AssetManager::GetAssetInfo(texture->GetID()).RuntimeAsset == false)
				return texture;
			/*
			if (!texture->GetPath().empty())
			{
				Count<Texture2D> newTexture = Texture2D::Create(texture->GetSpecification(), texture->GetPath());

				std::filesystem::path savePath = directory / (texture->GetPath().filename().string() + Utils::GetAssetExtensionString(AssetType::Texture));
				savePath = FileSystem::GenerateUniqueFileName(savePath);

				{
					auto asset = newTexture.As<Asset>();
					AssetManager::NewAsset(asset, savePath);
				}

				return newTexture;
			}
			else
			{
				PF_EC_ERROR("Not supported when texture is an embeded texture");

				Buffer buffer = texture.As<VulkanTexture2D>()->GetStoredDataAsBuffer();

				Count<Texture2D> newTexture = Texture2D::Create(texture->GetSpecification(),buffer);
				std::filesystem::path savePath = directory / ("EmbededTexture" + Utils::GetAssetExtensionString(AssetType::Texture));
				savePath = FileSystem::GenerateUniqueFileName(savePath);

				{
					auto asset = newTexture.As<Asset>();
					AssetManager::NewAsset(asset, savePath);
				}

				return newTexture;
			}
			*/
		}
		return texture;
	}
	static void CreateDiskMaterialFromRuntimeMaterial(AssetID id)
	{

		if (!AssetManager::HasAssetAndAssetType(id,AssetType::Material))
			return;
		if (AssetManager::IsDefaultAsset(id))
			return;


		const auto assetInfo = AssetManager::GetAssetInfo(id);

		if (assetInfo.RuntimeAsset == false)
			return;

		Count<Material> baseMaterial = AssetManager::GetAsset<Material>(id);

		if (ImGui::MenuItem("CreateDiskMaterialFromRuntimeMaterial"))
		{
				UI::ShowMessageBox("CreateDiskMaterialFromRuntimeMaterial", [id, assetInfo,baseMaterial]()
				{
					ImGui::Text(Project::GetActive()->GetProjectDirectory().filename().string().c_str());

					if (ConvertDiskMaterial.SavePath == "Materials/")
						ConvertDiskMaterial.SavePath += assetInfo.GetName();
					UI::AttributeInputText("Material Path", ConvertDiskMaterial.SavePath);

					if (ImGui::Button("Create"))
					{

						std::filesystem::path savedPath = Project::GetActive()->GetAssetDirectory() / ConvertDiskMaterial.SavePath;
						savedPath += Utils::GetAssetExtensionString(AssetType::Material);

						if (!FileSystem::Exists(savedPath.parent_path()))
							FileSystem::CreateDirectory(savedPath.parent_path());

						savedPath = FileSystem::GenerateUniqueFileName(savedPath);

						Count<Material> material = Count<Material>::Create();

						{
							auto asset = material.As<Asset>();
							AssetManager::NewAsset(asset, savedPath);
						}

						auto allTextures = baseMaterial->GetRenderMaterial()->GetAllTextures();


						for (auto& [biningName, texture] : allTextures)
						{ 
							auto savedTexture = MaterialCreateTextures(savedPath.parent_path(), texture);
							if(Renderer::GetWhiteTexture() != savedTexture && savedTexture != nullptr)
								material->GetRenderMaterial()->Set(biningName, savedTexture);
						}
						material->GetRenderMaterial()->CopyMaterialData(baseMaterial->GetRenderMaterial());
						ConvertDiskMaterial = {};
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();
					if (ImGui::Button("Cancel"))
					{
						ConvertDiskMaterial = {};
						ImGui::CloseCurrentPopup();
					}

				});
		}
	}
	bool AttributeDrawMaterialTable(Count<class MaterialTable> materialTable, Count<class MaterialTable> sourceMaterialTable)
	{
		bool modified = false;

		std::vector<uint32_t> clearMaterials;
		if (UI::AttributeTreeNode("Materials"))
		{

			for (auto& [index, material] : materialTable->GetMaterials())
			{
				std::string label = fmt::format("[Material {0}]", index);
				std::string id = fmt::format("{0}-{1}", label, index);

				ImGui::PushID(id.c_str());

				UI::PropertyAssetReferenceSettings settings;

				bool sourceHasMaterial = sourceMaterialTable->HasMaterial(index);

				bool mathcingMaterials = false;
				if (sourceHasMaterial)
				{
					auto sourceMaterial = sourceMaterialTable->GetMaterial(index);
					mathcingMaterials = sourceMaterial == material;
				}

				AssetID materialAssetHandle = 0;
				materialAssetHandle = material->GetID();
				settings.AdvanceToNextColumn = false;
				settings.AssetMemoryTypes = UIMemoryAssetTypes::Default;
				settings.OnRightClick = CreateDiskMaterialFromRuntimeMaterial; // not working yet
				settings.WidthOffset = ImGui::GetStyle().ItemSpacing.x + 28.0f;

				if (sourceHasMaterial && !mathcingMaterials)
					settings.WidthOffset += 40;

				if (UI::AttributeAssetReference(label, AssetType::Material, materialAssetHandle, settings))
				{
					modified = true;

					if (materialAssetHandle == 0)
						clearMaterials.push_back(index);
					else
						materialTable->SetMaterial(index, AssetManager::GetAsset<Material>(materialAssetHandle));

				}
				float prevItemHeight = ImGui::GetItemRectSize().y;

				ImGui::SameLine();
				if (material->GetAlbedoMap() != nullptr)
				{

					UI::ImageButton(material->GetAlbedoMap(), { prevItemHeight, prevItemHeight },
						{ material->GetAlbedoColor().x,material->GetAlbedoColor().y,material->GetAlbedoColor().z,1.0 });
				}
				else if (material->GetNormalMap() != nullptr && material->GetNormalTextureToggle() == true)
				{

					UI::ImageButton(material->GetNormalMap(), { prevItemHeight, prevItemHeight },
						{ material->GetAlbedoColor().x,material->GetAlbedoColor().y,material->GetAlbedoColor().z,1.0 });
				}
				else if (material->GetRoughnessMap() != nullptr)
				{

					UI::ImageButton(material->GetRoughnessMap(), ImVec2{ prevItemHeight, prevItemHeight },
						{ material->GetAlbedoColor().x,material->GetAlbedoColor().y,material->GetAlbedoColor().z,1.0 });
				}
				else if (material->GetMetalnessMap() != nullptr)
				{

					UI::ImageButton(material->GetMetalnessMap(), ImVec2{ prevItemHeight, prevItemHeight },
						{ material->GetAlbedoColor().x,material->GetAlbedoColor().y,material->GetAlbedoColor().z,1.0 });
				}
				else
				{

					UI::ImageButton(Renderer::GetWhiteTexture(), ImVec2{ prevItemHeight, prevItemHeight },
						{ material->GetAlbedoColor().x,material->GetAlbedoColor().y,material->GetAlbedoColor().z,1.0 });
				}
				if(!sourceHasMaterial && mathcingMaterials)
					ImGui::NextColumn();
				
				if (sourceHasMaterial && !mathcingMaterials)
				{
					ImGui::SameLine();
					if (ImGui::Button(UI::GenerateLabelID("X"), ImVec2{ prevItemHeight, prevItemHeight }))
					{
						modified = true;
						materialTable->SetMaterial(index, sourceMaterialTable->GetMaterial(index));
					}
					ImGui::NextColumn();
				}

				ImGui::PopID();
			}
			UI::EndTreeNode();
		}

		for (auto clear : clearMaterials)
			sourceMaterialTable->RemoveMaterial(clear);

		HandleModified(modified);
		return modified;
	}

	bool AttributeDrawUITable(Count<UITable> UITable)
	{

		if (UI::AttributeTreeNode("UITable"))
		{
			
			auto& layers = UITable->GetLayers();
			
			for (uint32_t layerIndex = 0; layerIndex < layers.size(); layerIndex++)
			{
				auto& layer = UITable->GetLayer(layerIndex);
				std::string layerLabel = fmt::format("[{} Index: {}]", layer.Name, layerIndex);
				std::string layerId = fmt::format("{0}-{1}", layerLabel, layerIndex);

				ImGui::PushID(layerId.c_str());

				
				if (UI::AttributeTreeNode(layerLabel, true, 3, 3))
				{
					if (ImGui::Button("+", ImVec2{ ImGui::GetContentRegionAvail().x,20 }))
					{
						layer.PushUI();
					}

					auto& panels = layer.GetUIPanels();

					for (uint32_t panelIndex = 0; panelIndex < panels.size(); panelIndex++)
					{
						auto panelInstance = panels[panelIndex];

						AssetID ID = 0;
						std::string panelLabel = fmt::format("[{} Index: {}]", "Not Set", panelIndex);
						if (panelInstance->GetUIPanel() != nullptr)
						{
							auto actualName =AssetManager::GetAssetInfo(panelInstance->GetUIPanel()).GetName();
							panelLabel = fmt::format("[{} Index: {}]", actualName, panelIndex);

							ID = panelInstance->GetUIPanel()->GetID();
						}
						std::string panelID = fmt::format("{0}-{1}", panelLabel, panelIndex);
						ImGui::PushID(panelID.c_str());

						if (UI::AttributeAssetReference(panelLabel, AssetType::UIPanel, ID))
						{
							if (AssetManager::HasAsset(ID)) // wath if its 0
								panelInstance->SetPanelInstance(AssetManager::GetAsset<UIPanel>(ID));
							else
								layer.PopIndex(panelIndex);

						}

						float prevItemHeight = ImGui::GetItemRectSize().y;

						ImGui::SameLine();
						if (ImGui::Button(UI::GenerateLabelID("X"), ImVec2{ prevItemHeight, prevItemHeight }))
						{
							layer.PopIndex(layerIndex);
						}

						if (panelInstance != nullptr)
						{
							UI::EditVariableInstance(panelInstance->GetVariableRegistryInstance());
						}
						ImGui::PopID();
					}

					UI::EndTreeNode();
				}

				ImGui::PopID();
			}
			UI::EndTreeNode();
		}
	}
	
}
