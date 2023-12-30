#include "Proofprch.h"
#include "DetailsPanel.h"
#include "Proof/ImGui/UiUtilities.h"
namespace Proof
{
	DetailsPanel::DetailsPanel(std::function<void()> renderDetails)
		:
		m_RenderDetails(renderDetails)
	{
	}
	void DetailsPanel::OnImGuiRender(const char* dsiplayName, bool& isOpen)
	{

		if (isOpen == false)
			return;
		PF_PROFILE_FUNC();
		UI::ScopedID customID(GetCustomPushID().Get());
		ImGui::Begin("Details", &isOpen);
		m_ImGuiWindow = ImGui::GetCurrentWindow();
		if (m_RenderDetails)
			m_RenderDetails();
		ImGui::End();
	}
	ImGuiWindow* DetailsPanel::GetImGuiWindow()
	{
		return m_ImGuiWindow;
	}
}

