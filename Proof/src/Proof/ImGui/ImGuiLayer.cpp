#include "Proofprch.h"
#include "ImGuiLayer.h"
#include "ImGUIOpenGL/imgui.h"
#include "ImGUIOpenGL/imgui_impl_glfw.h"
#include "ImGUIOpenGL/imgui_impl_opengl3.h"
#include "Platform/CurrentWindow.h"
static bool Docking = false;
namespace Proof {
	ImGuiLayer::ImGuiLayer() :
		Layer("ImGUI Layer")
	{
		
	}

	void ImGuiLayer::OnAttach() {
		Layer::OnAttach();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		
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
		ImGui::StyleColorsDark();
	}
}