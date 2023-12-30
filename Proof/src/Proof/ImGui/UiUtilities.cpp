#include "Proofprch.h"
#include "imgui_internal.h"
#include "Proof/Renderer/Image.h"
#include "Proof/Core/Application.h"
#include "Proof/Platform/Vulkan/VulkanImguiLayer.h"
#include "UIColors.h"
#include "UiUtilities.h"

namespace Proof::UI
{
	void HandleModified(bool value)
	{
		Application::Get()->GetImguiLayer()->UpdateCurrentModifiedType(value);
	}
	ImTextureID GetTextureID(Count<Image> texture)
	{
		Count<VulkanImguiLayer> layer = Application::Get()->GetImguiLayer().As< VulkanImguiLayer>();

		return layer->ToImguiImage(texture);
	}
	namespace Draw
	{

		void Underline(bool fullWidth, float offsetX, float offsetY)
		{
#if 1

			if (fullWidth)
			{
				if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)
					ImGui::PushColumnsBackground();
				else if (ImGui::GetCurrentTable() != nullptr)
					ImGui::TablePushBackgroundChannel();
			}

			const float width = fullWidth ? ImGui::GetWindowWidth() : ImGui::GetContentRegionAvail().x;
			auto cursor = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddLine(ImVec2(cursor.x + offsetX, cursor.y + offsetY),
				ImVec2(cursor.x + width, cursor.y + offsetY),
				Colours::Theme::BackgroundDark, 1.0f);

			if (fullWidth)
			{
				if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)
					ImGui::PopColumnsBackground();
				else if (ImGui::GetCurrentTable() != nullptr)
					ImGui::TablePopBackgroundChannel();
			}
#endif
		}
	}
	bool BeginPopup(const char* str_id, ImGuiWindowFlags flags)
	{
		bool opened = false;
		if (ImGui::BeginPopup(str_id, flags))
		{
			opened = true;
			// Fill background wiht nice gradient
			const float padding = ImGui::GetStyle().WindowBorderSize;
			const ImRect windowRect = UI::RectExpanded(ImGui::GetCurrentWindow()->Rect(), -padding, -padding);
			ImGui::PushClipRect(windowRect.Min, windowRect.Max, false);
			const ImColor col1 = ImGui::GetStyleColorVec4(ImGuiCol_PopupBg);// Colours::Theme::backgroundPopup;
			const ImColor col2 = UI::ColourWithMultipliedValue(col1, 0.8f);
			ImGui::GetWindowDrawList()->AddRectFilledMultiColor(windowRect.Min, windowRect.Max, col1, col1, col2, col2);
			ImGui::GetWindowDrawList()->AddRect(windowRect.Min, windowRect.Max, UI::ColourWithMultipliedValue(col1, 1.1f));
			ImGui::PopClipRect();

			// Popped in EndPopup()
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(0, 0, 0, 80));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));
		}

		return opened;
	}

	void EndPopup()
	{
		ImGui::PopStyleVar(); // WindowPadding;
		ImGui::PopStyleColor(); // HeaderHovered;
		ImGui::EndPopup();
	}
}

