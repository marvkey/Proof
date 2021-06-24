#include "Editor3D.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Proof {
	Editore3D::Editore3D() :
		Layer("Editor3D Layer")
	{
	}
	void Editore3D::OnAttach() {
		NewWorld();
		WoodenTexture = Texture2D::Create("PlayerSprite.png");
	}
	void Editore3D::NewWorld() {
		ActiveWorld = new World();
	}
	void Editore3D::OnUpdate(FrameTime DeltaTime) {
		Layer::OnUpdate(DeltaTime);
		Application::ViewPortWidth = _ViewPortSize.x;
		Application::ViewPortHeight = _ViewPortSize.y;
		Renderer2D::BeginContext(SceneCamera);
		Renderer2D::DrawQuad({Square1Pos.x+ XCount,Square1Pos.y,Square1Pos.z},Rotation,SquareScale,Square1Color,WoodenTexture);
		Renderer2D::DrawQuad({0.0,0.4,0},{0.2,0.6,0.3,1});
		Renderer2D::DrawQuad({0,0,0},{1,1,1,1});


		if(Input::IsKeyPressed(KeyBoardKey::A)){
			SceneCamera.SetPosition({SceneCamera.GetPosition().x-0.05,SceneCamera.GetPosition().y,SceneCamera.GetPosition().z});
		}

		if (Input::IsKeyPressed(KeyBoardKey::D)) {
			SceneCamera.SetPosition({SceneCamera.GetPosition().x+0.05,SceneCamera.GetPosition().y,SceneCamera.GetPosition().z});
		}
	}

	void Editore3D::OnImGuiDraw() {
		Layer::OnImGuiDraw();
		static bool EnableDocking = true;
		
		SetDocking(&EnableDocking);
		ViewPort();
		if (ImGui::Begin("Proof")) {
			if (ImGui::BeginChild("Tab Bar")) {
				ImGui::SameLine();
				if (ImGui::Button("Source Control")) {
				}
				ImGui::SameLine();
				if (ImGui::Button("Setting")) {
				}
				ImGui::SameLine();
				if (ImGui::Button("Play")) {
				}
				ImGui::SameLine();
				ImGui::Text("%.3f ms/frame %.1f FPS",FrameTime::GetFrameMS(),FrameTime::GetFrameFPS());
			}
			ImGui::EndChild();
		}
		ImGui::End();

		if (ImGui::Begin("Console")) {
			ImGui::TextColored({1.0f,1.0f,1.0f,0.3f},"This is the Console");
		}
		ImGui::End();
		if (ImGui::Begin("SceneHeirachy")) {
			ImGui::Text("One Entity");
		}
		ImGui::End();
		if (ImGui::Begin("Properties")) {
			ImGui::TextColored({1.0f,1.0f,1.0f,0.3f},"Properties");
			ImGui::Text("Location");
			ImGui::DragFloat("X",&Square1Pos.x,0.1f,0.0f,0.0f,"%.3f");
			ImGui::DragFloat("Y",&Square1Pos.y,0.1f,0.0f,0.0f,"%.3f");
			ImGui::Text("Scale");
			ImGui::DragFloat("x",&SquareScale.x,0.1f,0.0f,0.0f,"%.3f");
			ImGui::DragFloat("y",&SquareScale.y,0.1f,0.0f,0.0f,"%.3f");

			ImGui::DragFloat("RotatationX",&Rotation.x,0.3f,0.0f,0.0f,"%.3f");
			ImGui::DragFloat("RotatationY",&Rotation.y,0.3f,0.0f,0.0f,"%.3f");
			ImGui::DragFloat("RotatationZ",&Rotation.z,0.3f,0.0f,0.0f,"%.3f");

			ImGui::ColorEdit4("SquareColor",glm::value_ptr(Square1Color));
		}
		ImGui::End();

		if (ImGui::Begin("Content")) {
			ImGui::TextColored({1.0f,1.0f,1.0f,0.3f},"Player");
		}
		ImGui::End();
	}
	void Editore3D::ViewPort() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0,0});
		static bool Open = true;
		if (ImGui::Begin("ViewPort "),&Open,ImGuiWindowFlags_NoMove) {
			ImVec2 ViewPortPanelSize = ImGui::GetContentRegionAvail();
			if (_ViewPortSize != *((glm::vec2*)&ViewPortPanelSize)) {
				_ViewPortSize = {ViewPortPanelSize.x,ViewPortPanelSize.y};
				Window_ViewPortResize::Instance->EventHandled = true;
				Window_ViewPortResize::Instance->X = ViewPortPanelSize.x;
				Window_ViewPortResize::Instance->Y = ViewPortPanelSize.y;
			}
			
			if (ImGui::IsWindowFocused()&&ImGui::IsWindowHovered()) {
				Input::ViewPoartHovered = true;
			}
			else {
				Input::ViewPoartHovered = false;
			}

			uint32_t Text = Application::GetScreenBuffer()->GetTexture();
			ImGui::Image((void*)Text,ImVec2{_ViewPortSize.x,_ViewPortSize.y},ImVec2{0,1},ImVec2{1,0});
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Editore3D::SetDocking(bool * p_open) {
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen){
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize,0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2(0.0f,0.0f));
		ImGui::Begin("Proof Engine",p_open,window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();
		if (opt_fullscreen)
			ImGui::PopStyleVar(2);
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id,ImVec2(0.0f,0.0f),dockspace_flags);
		}
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem(" Save")) {

				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}
}