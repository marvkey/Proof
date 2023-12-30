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
		bool IsFocused() { return m_IsFocused; }
		bool IsHovered() { return m_IsHovered; }

		bool IsHoveredOrFocused() { return IsFocused() || IsHovered(); }
	private:
		bool m_IsHovered = false;
		bool m_IsFocused = false;
		ImGuiWindow* m_ImGuiWindow = nullptr;
		std::function<void()> m_RenderDetails;
	};
	
}