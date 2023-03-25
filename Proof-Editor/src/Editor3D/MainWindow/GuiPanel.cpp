#include "GuiPanel.h"
#include "Proof/Utils/PlatformUtils.h"

#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Renderer/UIRenderer/UIRenderer.h"
#include "Proof/Core/FrameTime.h"
namespace Proof
{
	GuiPanel::GuiPanel()
		:m_Camera(10, 12, 45, 0.1, 2000, { 0,0,10 })
	{
		m_UIPanel = Count<UIPanel>::Create();
		
		UIText text;
		text.Text = "Yo Wagwan";
		text.Postion = { 0,3 };
		text.Size = { 3,2 };

		m_UIPanel->AddText(text);

		UIButton button;
		button.Size = { 3,3 };
		button.TintColour = { 1,1,1,1};

		//m_UIPanel->AddButton(button);

	}

	void GuiPanel::ImGuiRender(FrameTime deltaTime)
	{
		if (m_ShowWindow == false)
			return;

		
		ImGui::Begin("GUI Panel", &m_ShowWindow);
		if (ImGui::IsWindowFocused())
		{
			Application::Get()->GetWindow()->SetWindowInputEvent(false);
			m_Camera.OnUpdate(deltaTime, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
			Application::Get()->GetWindow()->SetWindowInputEvent(false);
		}

		const void* Text = UiRenderer::DrawUI(m_UIPanel, m_Camera.m_Positon, m_Camera.m_Projection,m_Camera.m_View, ImGui::GetWindowWidth(), ImGui::GetWindowHeight()).SourceImage;

		ImGui::Image((ImTextureID)Text, ImVec2{ ImGui::GetWindowWidth(),ImGui::GetWindowHeight() }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		ImGui::End();
	}
}
