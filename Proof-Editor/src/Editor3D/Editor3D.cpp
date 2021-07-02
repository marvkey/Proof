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
		//WoodenTexture = Texture2D::Create("PlayerSprite.png");
	}
	void Editore3D::NewWorld() {
		ActiveWorld = new World();
		//Player  =ActiveWorld->CreateEntity("Base-Entity");
		//Player.AddComponent<MeshComponent>();
		//Player.GetComponent<MeshComponent>()->m_Mesh =&PlayerModel;
		m_WorldHierachy.SetContext(ActiveWorld);
		TestID = ECSTEST.Create();
		PF_ENGINE_INFO("%i",TestID);
		ECSTEST.AddComponent<TransformComponent>(TestID);
		if(ECSTEST.HasComponent<TransformComponent>(TestID)){
			PF_ENGINE_INFO("yeye");
		}
		if(ECSTEST.GetComponent<TransformComponent>(TestID) == nullptr){
			PF_ENGINE_INFO("Nullptr");
		}
		ECSTEST.RemoveComponent<TransformComponent>(TestID);
		
		//ECSTEST.Delete(TestID);
	}
	void Editore3D::OnUpdate(FrameTime DeltaTime) {
		Layer::OnUpdate(DeltaTime);
		Application::ViewPortWidth = _ViewPortSize.x;
		Application::ViewPortHeight = _ViewPortSize.y;


		EditorCamera.OnUpdate(DeltaTime);
		glm::mat4 Projection =glm::mat4(1.0f);
		Projection = glm::perspective(glm::radians(45.f),(float)CurrentWindow::GetWindowWidth() / (float)CurrentWindow::GetWindowHeight(),0.1f,100.0f);
		//Renderer3D::BeginContext(Projection,EditorCamera);
		//Renderer3D::Draw(*Player.GetComponent<MeshComponent>());
		//Renderer3D::EndContext();
	}

	void Editore3D::OnImGuiDraw() {
		Layer::OnImGuiDraw();
		static bool EnableDocking = true;
		
		SetDocking(&EnableDocking);
		ViewPort();
		m_WorldHierachy.ImGuiOnUpdate();
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
			
			if (ImGui::IsWindowFocused()) {
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