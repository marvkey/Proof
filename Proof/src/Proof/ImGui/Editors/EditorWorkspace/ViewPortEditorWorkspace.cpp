#include "Proofprch.h"
#include "ViewPortEditorWorkspace.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/ImGui/UI.h"
#include "Proof/Scene/World.h"
namespace Proof
{
	ViewPortEditorWorkspace::ViewPortEditorWorkspace(const std::string& viewPortname, ViewPortEditorData viewPortData) :
		EditorWorkspace(viewPortname), 
		m_ViewPortEditorData(viewPortData)
	{
		m_WorldRenderer = Count<WorldRenderer>::Create();
	}
	ViewPortEditorWorkspace::~ViewPortEditorWorkspace()
	{
	}
	void ViewPortEditorWorkspace::SetWorldContext(const Count<class World>& context)
	{
		m_WorldContext = context;
		m_WorldRenderer->SetContext(m_WorldContext);
	}
	void ViewPortEditorWorkspace::OnEvent(class Event& e)
	{
		m_Camera.OnEvent(e);
	}
	void ViewPortEditorWorkspace::OnUpdate(FrameTime ts)
	{
		m_Camera.SetViewportSize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		m_Camera.SetActive(IsFocused());
		m_Camera.OnUpdate(ts);
		m_WorldRenderer->SetViewportSize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		m_WorldContext->OnRenderEditor(m_WorldRenderer, ts, m_Camera);

	}
	void ViewPortEditorWorkspace::OnImGuiRender()
	{
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x,viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x,viewportMaxRegion.y + viewportOffset.y };
		ImVec2 currentviewPortPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewPortSize != *((glm::vec2*)&currentviewPortPanelSize))
		{
			m_ViewPortSize = { currentviewPortPanelSize.x,currentviewPortPanelSize.y };
		}
		UI::Image(m_WorldRenderer->GetFinalPassImage(), ImVec2{ m_ViewPortSize.x,m_ViewPortSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		if (m_ViewPortEditorData.HandleOnImGuiDrop)
			m_ViewPortEditorData.HandleOnImGuiDrop();
	}
	void ViewPortEditorWorkspace::OnWindowStylePush()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

	}
	void ViewPortEditorWorkspace::OnWindowStylePop()
	{
		ImGui::PopStyleVar();
	}

}
