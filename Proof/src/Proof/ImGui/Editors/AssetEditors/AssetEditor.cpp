#include "Proofprch.h"
#include "AssetEditor.h"
#include "Proof/Core/FrameTime.h"
#include "ProoF/Asset/Asset.h"
#include "ProoF/Asset/AssetManager.h"
#include "Proof/ImGui/UI.h"
#include "MaterialEditorPanel.h"
#include "ParticleSystemEditorPanel.h"
#include "MeshEditorPanel.h"
#include "MeshColliderEditorPanel.h"
#include "Proof/ImGui/UiUtilities.h"

namespace Proof 
{
	AssetEditor::AssetEditor(const char* id)
		: m_Id(id), m_MinSize(200, 400), m_MaxSize(FLT_MAX, FLT_MAX)
	{
	}
	

	void AssetEditor::SetOpen(bool isOpen)
	{
		m_IsOpen = isOpen;
		if (!m_IsOpen)
			OnClose();
		else
			OnOpen();
	}

	void AssetEditor::SetMinSize(uint32_t width, uint32_t height)
	{
		if (width <= 0) width = 200;
		if (height <= 0) height = 400;

		m_MinSize = ImVec2(float(width), float(height));
	}

	void AssetEditor::SetMaxSize(uint32_t width, uint32_t height)
	{
		if (width <= 0) width = 2000;
		if (height <= 0) height = 2000;
		if (float(width) <= m_MinSize.x) width = (uint32_t)(m_MinSize.x * 2.f);
		if (float(height) <= m_MinSize.y) height = (uint32_t)(m_MinSize.y * 2.f);

		m_MaxSize = ImVec2(float(width), float(height));
	}

	void AssetEditor::Render()
	{
		if (!m_IsOpen)
			return;

		bool was_open = m_IsOpen;
		// TODO SetNextWindowSizeConstraints requires a max constraint that's above 0. For now we're just setting it to a large value
		{
			UI::ScopedID (m_PushID.Get());
			OnWindowStylePush();
			ImGui::SetNextWindowSizeConstraints(m_MinSize, m_MaxSize);
			ImGui::Begin(m_TitleAndId.c_str(), &m_IsOpen, GetWindowFlags());
			m_CurrentSize = ImGui::GetWindowSize();
			if (ImGui::IsWindowFocused())
				m_IsFocused = true;
			else
				m_IsFocused = false;
			OnWindowStylePop();
			{
				OnImGuiRender();
			}
			ImGui::End();
		}
		if (was_open && !m_IsOpen)
			OnClose();
	}

	void AssetEditor::SetTitle(AssetInfo assetInfo)
	{
		m_TitleAndId = assetInfo.GetName() + "###" + m_Id + std::to_string( assetInfo.ID).c_str();
		//m_TitleAndId = newTitle + "###" + m_Id;
	}

	void AssetEditorPanel::RegisterDefaultEditors()
	{
		RegisterEditor(AssetType::Material);
		RegisterEditor(AssetType::PhysicsMaterial);
		RegisterEditor(AssetType::ParticleSystem);
		RegisterEditor(AssetType::Mesh);
		RegisterEditor(AssetType::MeshCollider);
	}

	void AssetEditorPanel::UnregisterAllEditors()
	{
		s_SceneContext = nullptr;
		s_Editors.clear();
	}

	void AssetEditorPanel::OnUpdate(FrameTime ts)
	{
		for (auto& kv : s_Editors)
		{
			for (auto& [id, panel] : kv.second)
			{
				if(panel->m_IsOpen == true)
					panel->OnUpdate(ts);
			}
		}
	}

	void AssetEditorPanel::OnEvent(Event& e)
	{
		for (auto& kv : s_Editors)
		{
			for (auto& [id, panel] : kv.second)
			{
				if (panel->m_IsOpen == true && panel->IsFocused())
					panel->OnEvent(e);
			}
		}
	}

	void AssetEditorPanel::SetSceneContext(const Count<World>& context)
	{
		s_SceneContext = context;

		for (auto& kv : s_Editors)
		{
			for (auto& [id, panel] : kv.second)
				panel->SetWorldContext(context);
		}
	}

	void AssetEditorPanel::OnImGuiRender()
	{
		for (auto& kv : s_Editors)
		{
			for (auto& [id, panel] : kv.second)
				panel->Render();
		}
	}

	void AssetEditorPanel::OpenEditor(const Count<Asset>& asset)
	{
		if (asset == nullptr)
			return;

		if (s_Editors.find(asset->GetAssetType()) == s_Editors.end())
		{
			PF_EC_ERROR("Asset Editore panel does not support {}", EnumReflection::EnumString( asset->GetAssetType()))
			return;
		}

		if (!s_Editors[asset->GetAssetType()].contains(asset->GetID()))
		{
			switch (asset->GetAssetType())
			{
				case Proof::AssetType::Mesh:
					s_Editors[asset->GetAssetType()][asset->GetID()] = Count<MeshEditorPanel>::Create();
					break;
				case Proof::AssetType::Texture:
					break;
				case Proof::AssetType::Material:
					s_Editors[asset->GetAssetType()][asset->GetID()] = Count<MaterialEditorPanel>::Create();
					break;
				case Proof::AssetType::MeshSourceFile:
					break;
				case Proof::AssetType::PhysicsMaterial:
					s_Editors[asset->GetAssetType()][asset->GetID()] = Count<PhysicsMaterialEditorPanel>::Create();
					break;
				case Proof::AssetType::TextureSourceFile:
					break;
				case Proof::AssetType::Font:
					break;
				case Proof::AssetType::FontSourceFile:
					break;
				case Proof::AssetType::Prefab:
					break;
				case Proof::AssetType::UIPanel:
					break;
				case Proof::AssetType::ParticleSystem:
					s_Editors[asset->GetAssetType()][asset->GetID()] = Count<ParticleSystemEditorPanel>::Create();
					break;
				case Proof::AssetType::MeshCollider:
					s_Editors[asset->GetAssetType()][asset->GetID()] = Count<MeshColliderEditorPanel>::Create();
					break;
				default:
					break;
			}
		}
		const auto& editor = s_Editors[asset->GetAssetType()][asset->GetID()];
		if (editor->IsOpen())
			editor->SetOpen(false);

		const auto& metadata = AssetManager::GetAssetInfo(asset->GetID());
		editor->SetTitle(metadata);
		editor->SetAsset(AssetManager::GetAsset<Asset>(asset->GetID()));
		editor->SetWorldContext(s_SceneContext);
		editor->SetOpen(true);
	}

	std::unordered_map<AssetType, std::unordered_map<UUID, Count<AssetEditor>>> AssetEditorPanel::s_Editors;
	Count<World> AssetEditorPanel::s_SceneContext = nullptr;
}
