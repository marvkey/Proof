#include "Proofprch.h"
#include "ImGuiLayer.h"
#include "ImGUIOpenGL/imgui.h"
#include "Platform/CurrentWindow.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/MouseEvent.h"
namespace Proof {
	ImGuiLayer::ImGuiLayer() :
		Layer("ImGUI")
	{

	}

	void ImGuiLayer::OnUpdate(FrameTime DeltaTime) {

	}
	void ImGuiLayer::OnAttach() {
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(CurrentWindow::GetWindow(), true);
		SetDarkTheme();
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImGuiLayer::OnDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void ImGuiLayer::OnImGuiDraw(){
		static bool Show = true;
		ImGui::ShowDemoWindow(&Show);
	}
	void ImGuiLayer::Begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void ImGuiLayer::End() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void ImGuiLayer::SetDarkTheme() {
		ImGui::StyleColorsDark();
	}
}