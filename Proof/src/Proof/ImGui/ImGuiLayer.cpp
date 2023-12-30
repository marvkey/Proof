#include "Proofprch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Platform/Vulkan/VulkanImguiLayer.h"
namespace Proof
{
	ImGuiLayer::ImGuiLayer() :
		Layer("ImGUI Layer") {
	}
	ImGuiLayer::ImGuiLayer(const std::string& name) :
		Layer(name)
	{

	}
	
	void ImGuiLayer::End()
	{
		for (auto& [key, messageBoxData] : m_MessageBoxes)
		{
			if (messageBoxData.ShouldOpen && !ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId))
			{
				ImGui::OpenPopup(messageBoxData.Title.c_str());
				messageBoxData.ShouldOpen = false;
				messageBoxData.IsOpen = true;
			}

			if (!messageBoxData.IsOpen)
				continue;

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			ImGui::SetNextWindowSize(ImVec2{ (float)messageBoxData.Width, (float)messageBoxData.Height });

			if (ImGui::BeginPopupModal(messageBoxData.Title.c_str(), &messageBoxData.IsOpen, ImGuiWindowFlags_AlwaysAutoResize))
			{
				if (messageBoxData.Flags & UI::UIMessageBoxBit::UserFunc )
				{
					PF_CORE_ASSERT(messageBoxData.UserRenderFunction, "No render function provided for message box!");
					messageBoxData.UserRenderFunction();
				}
				else
				{
					ImGui::TextWrapped(messageBoxData.Body.c_str());

					if (messageBoxData.Flags & UI::UIMessageBoxBit::OkButton )
					{
						if (ImGui::Button("Ok"))
							ImGui::CloseCurrentPopup();

						if (messageBoxData.Flags & UI::UIMessageBoxBit::CancelButton)
							ImGui::SameLine();
					}

					if (messageBoxData.Flags & UI::UIMessageBoxBit::CancelButton && ImGui::Button("Cancel"))
					{
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}
		}

	}
	void ImGuiLayer::PopModified()
	{
		PF_CORE_ASSERT(!m_ModifiedTypes.empty(), "Modified types are empty");
		m_ModifiedTypes.pop();
	}
	void ImGuiLayer::UpdateCurrentModifiedType(bool value)
	{
		if (m_ModifiedTypes.empty())
			return;
		bool& modified = *m_ModifiedTypes.top();
		modified |= value;
	}
	void ImGuiLayer::SetDarkTheme() {

		auto& style = ImGui::GetStyle();
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f,0.1f,0.1f,1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.15f,0.15f,0.15f,1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.2f,0.2f,0.5f,1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.2f,0.2f,0.5f,1.0f };

		// Buttons Done
		colors[ImGuiCol_Button] = ImVec4{ 0.15f,0.15f,0.15f,1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.2f,0.2f,0.5f,1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.2f,0.2f,0.5f,1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f,0.2f,0.21f,1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f,0.305f,0.31f,1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.1f,0.1505f,0.151f,1.0f };


		// Tabs Done
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.2f,0.2f,0.5f,1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.2f,0.2f,0.2f,1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.1f,0.1f,0.1f,1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.1305f,0.12f,0.12f,1.0f };

		// Title Done
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.1f,0.1f,0.121f,1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.10f,0.12f,0.2f,1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.1095f,0.10f,0.1f,1.0f };
		//colors[ImGuiCol_TextDisabled]
		style.FrameRounding = 12;
		style.PopupRounding = 12;
		style.TabBorderSize = 1;
		style.GrabRounding = 12;
		style.PopupBorderSize = 0;
		style.ScrollbarSize = 15;
		style.WindowRounding = 0.0f;

	}
	Count<ImGuiLayer> ImGuiLayer::Create()
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanImguiLayer>::Create();
		}
	}
	
}