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
#include "Proof/Events/Event.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Input/Input.h"
#include "Proof/ImGui/Editors/AssetEditors/PrefabEditor.h"
namespace Proof 
{
	AssetEditor::AssetEditor(const char* id)
		: m_Id(id), m_MinSize(200, 400), m_MaxSize(FLT_MAX, FLT_MAX)
	{
	}
	

	void AssetEditor::OnUpdate(FrameTime ts)
	{
		m_SaveCountDown -= ts;
		if (m_SaveCountDown <= 0)
		{
			Save();
			m_SaveCountDown = SavedPresetSeconds;
		}
	}

	void AssetEditor::OnEvent(Event& e)
	{
		if (!IsHoveredOrFocused())
			return;

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyClickedEvent>([&](auto& e) 
		{
				PF_CORE_ASSERT(false);
			bool control = Input::IsKeyPressed(KeyBoardKey::LeftControl) || Input::IsKeyPressed(KeyBoardKey::RightControl);
			
			switch (e.GetKey())
			{
				case KeyBoardKey::S:
				{
					if (control) 
					{
						Save();
						m_SaveCountDown = SavedPresetSeconds;
						return true;
					}
				}
				break;
			default:
				break;
			}
			return false;
		});
	}

	void AssetEditor::SetOpen(bool isOpen)
	{
		m_IsOpen = isOpen;
		if (!m_IsOpen)
			OnClose();
		else
			OnOpen();
	}

	ImGuiWindow* AssetEditor::GetImGuiWindow()
	{
		return m_ImGuiWindow;
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

	std::string AssetEditor::GetBaseDockspace()
	{
		return m_TitleAndId + "Dockspace";
	}

	void AssetEditor::MenuBar()
	{
		ImGui::BeginMenuBar();
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save", "ctrl+s"))
				{
					m_SaveCountDown = SavedPresetSeconds;
					Save();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("SetDefaultLayout"))
					SetDefaultLayout();
				ImGui::EndMenu();

			}
		}
		ImGui::EndMenuBar();
	}

	void AssetEditor::Render()
	{
		if (!m_IsOpen)
			return;

		bool was_open = m_IsOpen;
		ImGuiWindowFlags window_flags  = 0;
		window_flags |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse;
		if (!IsSaved())
			window_flags |= ImGuiWindowFlags_UnsavedDocument;

		// TODO SetNextWindowSizeConstraints requires a max constraint that's above 0. For now we're just setting it to a large value
		{
			UI::ScopedID(m_PushID.Get());
			OnWindowStylePush();
			ImGui::SetNextWindowSizeConstraints(m_MinSize, m_MaxSize);
			ImGuiWindowClass window_class2;
			window_class2.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoDockingSplitOther | ImGuiDockNodeFlags_NoDockingSplitMe;
			window_class2.DockingAllowUnclassed = false;
			ImGui::Begin(m_TitleAndId.c_str(), &m_IsOpen, GetWindowFlags() | window_flags);

			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				auto window = ImGui::GetCurrentWindow();
				if (window->TitleBarRect().Contains(ImGui::GetMousePos()))
				{
					auto monitor = ImGui::GetPlatformIO().Monitors[window->Viewport->PlatformMonitor];
					ImGui::SetWindowPos(m_TitleAndId.c_str(), {monitor.WorkPos});
					ImGui::SetWindowSize(m_TitleAndId.c_str(), { monitor.WorkSize });
				}
			}
			m_CurrentSize = ImGui::GetWindowSize();
			m_IsFocused = ImGui::IsWindowFocused(ImGuiHoveredFlags_RootAndChildWindows);
			m_IsHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
			OnWindowStylePop();
			MenuBar();
			m_ImGuiWindow = ImGui::GetCurrentWindow();

			ImGuiID dockspace_id = ImGui::GetID(GetBaseDockspace().c_str());
			ImGui::DockSpace(dockspace_id);
			//ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_PassthruCentralNode , &ImGui::GetCurrentWindow()->WindowClass);
			
			{
				OnImGuiRender();
			}
			if (m_FirstRunImGui)
			{
				SetDefaultLayout();
				m_FirstRunImGui = false;
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
		RegisterEditor(AssetType::Prefab);
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
				if (panel->m_IsOpen == true && panel->IsHoveredOrFocused())
				{
					panel->OnEvent(e);
				}
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
			PF_EC_ERROR("Asset Editor panel does not support {}", EnumReflection::EnumString( asset->GetAssetType()))
			return;
		}

		if (!s_Editors[asset->GetAssetType()].contains(asset->GetID()))
		{
			switch (asset->GetAssetType())
			{
				case Proof::AssetType::Mesh:
					//s_Editors[asset->GetAssetType()][asset->GetID()] = Count<MeshEditorPanel>::Create();
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
					s_Editors[asset->GetAssetType()][asset->GetID()] = Count<PrefabEditorPanel>::Create();
					break;
				case Proof::AssetType::UIPanel:
					break;
				case Proof::AssetType::ParticleSystem:
					//s_Editors[asset->GetAssetType()][asset->GetID()] = Count<ParticleSystemEditorPanel>::Create();
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
