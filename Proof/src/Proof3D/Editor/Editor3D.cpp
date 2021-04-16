#include "Proofprch.h"
#include "Editor3D.h"
#include "ImGUIOpenGL/imgui.h"
#include "ImGUIOpenGL/imgui_impl_glfw.h"
#include "ImGUIOpenGL/imgui_impl_opengl3.h"
#include "Platform/CurrentWindow.h"

static bool Docking = false;
namespace Proof {
	Editore3D::Editore3D() :
		Layer("Editor 3D")
	{

	}
	void Editore3D::OnUpdate(FrameTime DeltaTime) {
	}
	void Editore3D::OnAttach() {

	}
	void Editore3D::OnDetach() {
	}
	void Editore3D::OnImGuiDraw() {
		Layer::OnImGuiDraw();

		if (Docking == true)
			SetDocking(&Docking);

		if (ImGui::IsKeyDown((int)KeyBoardKey::LeftAlt) || ImGui::IsKeyDown((int)KeyBoardKey::RightAlt))
			Docking = true;
		if (ImGui::IsKeyReleased((int)KeyBoardKey::LeftAlt) || ImGui::IsKeyReleased((int)KeyBoardKey::RightAlt))
			Docking = false;

		if (ImGui::Begin("Proof", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse)) {
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Open", "Ctrl+O")) {}
					if (ImGui::MenuItem("Save", "Ctrl+S")) { PF_ENGINE_INFO("HAS BEEN SAVE"); }
					if (ImGui::MenuItem("Close", "Ctrl+W")) { glfwSetWindowShouldClose(CurrentWindow::GetWindow(), true); }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit")) {
					if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
					if (ImGui::MenuItem("redo", "Ctrl+Y")) {}
					ImGui::NewLine();

					ImGui::Text("Functions");
					if (ImGui::MenuItem("Cut", "Ctrl+x")) { PF_ENGINE_INFO("Cut"); }
					if (ImGui::MenuItem("Copy", "Ctrl+C")) { PF_ENGINE_INFO("Copy"); }
					if (ImGui::MenuItem("Paste", "Ctrl+v")) { PF_ENGINE_INFO("Paste"); }
					if (ImGui::MenuItem("Docking", "Ctrl", &Docking)) { PF_ENGINE_INFO("Docking Enabled"); }
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();

			}

			if (ImGui::BeginChild("Tab Bar")) {
				if (ImGui::Button("Save Current")) {

				}
				ImGui::SameLine();
				if (ImGui::Button("Source Control")) {

				}
				ImGui::SameLine();

				if (ImGui::Button("Setting")) {

				}
				ImGui::SameLine();
				if (ImGui::Button("Play")) {

				}
				ImGui::EndChild();
			}
		}
		ImGui::End();
		//LogConsole();
		//ImGui::ShowDemoWindow();
	}

	void Editore3D::LogConsole() {
		if (ImGui::Begin("Log")) {
			ImGui::Text("Welcome to the file");
			ImGui::TextColored(ImVec4{ 0.4,0.3,0.7,0.9 }, "THAT IS A GG");
		}
		ImGui::End();
	}

	void Editore3D::ContentBrowser() {
		if (ImGui::Begin("Content Browser")) {
			ImGui::Text("There is a football and boots");
		}
		ImGui::End();
	}

	void Editore3D::WorldObjects() {
		if (ImGui::Begin("Scene Object")) {
			ImGui::Text("Goal post");
			ImGui::Text("Boots");
		}
		ImGui::End();
	}

	void Editore3D::SetDocking(bool* p_open)
	{
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace", p_open, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}


		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Options")) {
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();

				if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
					*p_open = false;
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();
	}
}