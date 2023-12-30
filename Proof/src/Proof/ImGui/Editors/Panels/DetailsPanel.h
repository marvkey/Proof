#pragma once
#include "Panel.h"
#include <functional>
#include <imgui_internal.h>
namespace Proof
{
	class DetailsPanel : public Panel
	{
	public:
		DetailsPanel(std::function<void()> renderDetails = nullptr);
		void OnImGuiRender(const char* dsiplayName, bool& isOpen);
		ImGuiWindow* GetImGuiWindow();
	private:
		ImGuiWindow* m_ImGuiWindow = nullptr;
		std::function<void()> m_RenderDetails;
	};
	
}