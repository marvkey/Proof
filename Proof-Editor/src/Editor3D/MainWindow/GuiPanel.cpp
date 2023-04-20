#include "GuiPanel.h"
#include "Proof/Utils/PlatformUtils.h"

#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Renderer/UIRenderer/UIRenderer.h"
#include "Proof/Core/FrameTime.h"
namespace Proof
{
	GuiPanel::GuiPanel()
		:
		m_Camera(45)
	{
		m_UIPanel = Count<UIPanel>::Create();
		
		UIText text;
		text.Text = "Yo Wagwan";
		text.Postion = { 1,1 };
		text.Size = { 1,1 };

		m_UIPanel->AddText(text);

		UIButton button;
		button.Size = { 1,1 };
		button.TintColour = { 1,1,1,1};
		text.Postion = { 0,0 };

		//m_UIPanel->AddButton(button);
		//m_Camera.
	}

	void GuiPanel::ImGuiRender(FrameTime deltaTime)
	{
		if (m_ShowWindow == false)
			return;

		
		ImGui::Begin("GUI Panel", &m_ShowWindow);
		if (ImGui::IsWindowFocused())
		{
			Application::Get()->GetWindow()->SetWindowInputEvent(true);
			m_Camera.SetViewportSize(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
			m_Camera.OnUpdate(deltaTime);
			Application::Get()->GetWindow()->SetWindowInputEvent(false);
		}

		const void* Text = UiRenderer::DrawUI(m_UIPanel, m_Camera.GetPosition(), glm::mat4(1.0f), glm::mat4(1.0f),
				ImGui::GetWindowWidth(), ImGui::GetWindowHeight()).SourceImage;

		ImGui::Image((ImTextureID)Text, ImVec2{ ImGui::GetWindowWidth(),ImGui::GetWindowHeight() }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		ImGui::End();
	}
}
