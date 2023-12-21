#include "Proofprch.h"
#include "EditorWorkspace.h"

namespace Proof
{
	EditorWorkspace::EditorWorkspace(const std::string& titleAndID)
		:
		m_TitleAndID(titleAndID)
	{
	}
	EditorWorkspaceManager::~EditorWorkspaceManager()
	{
	}
	void EditorWorkspace::Render()
	{
		if (!m_IsOpen)
			return;

		bool was_open = m_IsOpen;
		// TODO SetNextWindowSizeConstraints requires a max constraint that's above 0. For now we're just setting it to a large value
		{
			OnWindowStylePush();
			ImGui::SetNextWindowSizeConstraints({200,400}, {FLT_MAX,FLT_MAX });
			ImGui::Begin(m_TitleAndID.c_str(), &m_IsOpen, GetWindowFlags());
			m_CurrentSize = ImGui::GetWindowSize();
			if (ImGui::IsWindowFocused())
				m_IsFocused = true;
			else
				m_IsFocused = false;

			m_IsHovered = ImGui::IsWindowHovered();
			OnWindowStylePop();
			{
				OnImGuiRender();
			}
			ImGui::End();
		}
		if (was_open && !m_IsOpen)
			OnClose();
	}

	void EditorWorkspace::SetOpen(bool isOpen)
	{
		m_IsOpen = isOpen;
		if (!m_IsOpen)
			OnClose();
		else
			OnOpen();
	}
	
	void EditorWorkspaceManager::OnImGuiRender()
	{
		for (auto& [id, panelData] : m_EditorWorkspaces)
		{
			if (!panelData.IsOpen)
				continue;
			if (panelData.EditorWorkspace->IsOpen() != panelData.IsOpen)
				panelData.EditorWorkspace->SetOpen(panelData.IsOpen);

			panelData.EditorWorkspace->Render();
			panelData.IsOpen = panelData.EditorWorkspace->IsOpen();
		}
	}
	void EditorWorkspaceManager::OnEvent(Event& e)
	{
		for (auto& [id, panelData] : m_EditorWorkspaces)
		{
			if (!panelData.IsOpen )
				continue;
			if(panelData.EditorWorkspace->IsFocusedOrHovered())
				panelData.EditorWorkspace->OnEvent(e);
		}
	}
	void EditorWorkspaceManager::OnUpdate(FrameTime ts)
	{
		for (auto& [id, panelData] : m_EditorWorkspaces)
		{
			if (!panelData.IsOpen)
				continue;
			panelData.EditorWorkspace->OnUpdate(ts);
		}
	}
	void EditorWorkspaceManager::SetWorldContext(const Count<class World>& context)
	{
		for (auto& [id, panelData] : m_EditorWorkspaces)
		{
			panelData.EditorWorkspace->SetWorldContext(context);
		}
	}

}