#include "Proofprch.h"
#include "ImGuiLayer.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Proof/Core/CurrentWindow.h"

#include "ImGuizmo.h"
namespace Proof {
	ImGuiLayer::ImGuiLayer() :
		Layer("ImGUI Layer")
	{
		
	}

	void ImGuiLayer::OnAttach() {
		Layer::OnAttach();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
		
		/* for fonts got to cherno video from 8 mINute on making editor look GOOd*/

		io.Fonts->AddFontFromFileTTF("Assets/Fonts/Poppins/Poppins-Bold.ttf",18.0f);
		io.FontDefault =io.Fonts->AddFontFromFileTTF("Assets/Fonts/Poppins/Poppins-Regular.ttf",17.0f);
		SetDarkTheme();
		ImGui_ImplGlfw_InitForOpenGL(CurrentWindow::GetWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImGuiLayer::OnDetach() {
		Layer::OnDetach();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}
	
	void ImGuiLayer::End() {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)CurrentWindow::GetWindowWidth(), (float)CurrentWindow::GetWindowHeight());
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}		
	}
	void ImGuiLayer::SetDarkTheme() {

		auto& style = ImGui::GetStyle();
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{0.1f,0.1f,0.1f,1.0f};

		// Headers
		colors[ImGuiCol_Header] = ImVec4{0.15f,0.15f,0.15f,1.0f};
		colors[ImGuiCol_HeaderHovered] = ImVec4{0.2f,0.2f,0.5f,1.0f};
		colors[ImGuiCol_HeaderActive] = ImVec4{0.2f,0.2f,0.5f,1.0f};

		// Buttons Done
		colors[ImGuiCol_Button] = ImVec4{0.15f,0.15f,0.15f,1.0f};
		colors[ImGuiCol_ButtonHovered] = ImVec4{0.2f,0.2f,0.5f,1.0f};
		colors[ImGuiCol_ButtonActive] = ImVec4{0.2f,0.2f,0.5f,1.0f};

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{0.2f,0.2f,0.21f,1.0f};
		colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f,0.305f,0.31f,1.0f};
		colors[ImGuiCol_FrameBgActive] = ImVec4{0.1f,0.1505f,0.151f,1.0f};


		// Tabs Done
		colors[ImGuiCol_Tab] = ImVec4{0.15f,0.1505f,0.151f,1.0f};
		colors[ImGuiCol_TabHovered] = ImVec4{0.2f,0.2f,0.5f,1.0f};
		colors[ImGuiCol_TabActive] = ImVec4{0.2f,0.2f,0.2f,1.0f};
		colors[ImGuiCol_TabUnfocused] = ImVec4{0.1f,0.1f,0.1f,1.0f};
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.1305f,0.12f,0.12f,1.0f};

		// Title Done
		colors[ImGuiCol_TitleBg] = ImVec4{0.1f,0.1f,0.121f,1.0f};
		colors[ImGuiCol_TitleBgActive] = ImVec4{0.10f,0.12f,0.2f,1.0f};
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.1095f,0.10f,0.1f,1.0f};
		style.FrameRounding = 12;
		style.PopupRounding =12;
		style.TabBorderSize =1;
		style.PopupRounding =12;
		style.GrabRounding =12;
		style.PopupBorderSize =0;
		style.ScrollbarSize =15;
	}
}