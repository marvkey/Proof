#include "Editor3D.h"
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ImGuizmo.h"
#include "Proof/Utils/PlatformUtils.h"
#include "Proof/Renderer/Renderer.h"

#include "Proof/Math/MathResource.h"
#include "Proof/Scene/Material.h"
#include "Proof/Resources/Asset/MaterialAsset.h"
#include "windows.h"
#include "Proof/Scene/Script.h"
//#include "../Game/Proof-Game/Proof-Game/src/generated/AllFiles.h"
//#include "../Game/Proof-Game/Proof-Game/src/generated/Init.h"
#include <Windows.h>
#include <stdio.h> 
#include "ImGUIAPI.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Utils/PlatformUtils.h"
#include "MainWindow/SceneRendererUI.h"
#include "MainWindow/MaterialEditorPanel.h"
#include "Proof/Resources/Math/Random.h"
#include <string>
namespace Proof
{
	Editore3D::Editore3D():
		Layer("Editor3D Layer") 	{	}
	Editore3D::~Editore3D() {
	}
	void Editore3D::OnAttach() {
	//	std::string name =std::format("{} {}","wagwan");
		m_CheckeboardTexture = Texture2D::Create("Assets/Textures/CheckeboardTexture.jpg");
		
	

		//m_WorldHierachy.SetBrowserPanel(&m_CurrentContentBrowserPanel);

		CubeMapPaths.emplace_back("Assets/Textures/skybox/right.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/left.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/bottom.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/top.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/front.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/back.jpg");

		ActiveWorld = CreateCount<World>();
		SceneSerializer scerelizer(ActiveWorld.get());
		if (scerelizer.DeSerilizeText("content/Levels/Lightest.ProofAsset") == true) {
			PF_ENGINE_INFO("%s Deserilize perfectly",ActiveWorld->GetName().c_str());
			m_WorldHierachy.SetContext(ActiveWorld.get());
			AssetManager::NotifyOpenedNewLevel(scerelizer.GetAssetLoadID());
		}
		m_WorldHierachy.SetContext(ActiveWorld.get());
		m_WorldRenderer = WorldRenderer(ActiveWorld.get(), CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight());
		// cannot be setting it to window size and stuff innit
		float skyboxVertices[] = {
					 // positions          
			-1.0f,1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f,1.0f,-1.0f,
			-1.0f,1.0f,-1.0f,

			-1.0f,-1.0f,1.0f,
			-1.0f,-1.0f,-1.0f,
			-1.0f,1.0f,-1.0f,
			-1.0f,1.0f,-1.0f,
			-1.0f,1.0f,1.0f,
			-1.0f,-1.0f,1.0f,

			1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,1.0f,
			1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,
			1.0f,1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,

			-1.0f,-1.0f,1.0f,
			-1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,
			1.0f,-1.0f,1.0f,
			-1.0f,-1.0f,1.0f,

			-1.0f,1.0f,-1.0f,
			1.0f,1.0f,-1.0f,
			1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,
			-1.0f,1.0f,1.0f,
			-1.0f,1.0f,-1.0f,

			-1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			-1.0f,-1.0f,1.0f,
			1.0f,-1.0f,1.0f
		};

		m_SkyBoxShader = Shader::GetOrCreate("SkyBox Shader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/CubeMapShader.shader");
		m_SkyBoxBuffer = VertexBuffer::Create(&skyboxVertices,sizeof(skyboxVertices));
		m_SkyBoxVertexArray = VertexArray::Create();
		m_SkyBoxVertexArray->AddData(0,3,3 * sizeof(float),(void*)0);

		m_SkyBoxShader->Bind();
		m_SkyBoxShader->SetInt("skybox",0);
		//
		//
		m_CubeMap = CubeMap::Create(CubeMapPaths);

		m_PlayButtonTexture = Texture2D::Create("Resources/Icons/MainPanel/PlayButton.png");
		m_PauseButtonTexture = Texture2D::Create("Resources/Icons/MainPanel/PauseButton .png");
		m_SimulateButtonTexture = Texture2D::Create("Resources/Icons/MainPanel/SimulateButton.png");
		m_StopButtonTexture =Texture2D::Create("Resources/Icons/MainPanel/StopButton.png");

		//PF_ENGINE_INFO("number ofscript is %i",ScriptDetail::GetScriptRegisry().size());
	}
	void Editore3D::OnDetach() {
		if (ActiveWorld != nullptr) {
			SceneSerializer scerelizer(ActiveWorld.get());
			scerelizer.SerilizeText(ActiveWorld->GetPath());
		}
	}

	void Editore3D::OnUpdate(FrameTime DeltaTime) {
		Layer::OnUpdate(DeltaTime);
		if (Input::IsKeyClicked(KeyBoardKey::L)) {
			GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
		}
		if (Input::IsKeyClicked(KeyBoardKey::R)) {
			GuizmoType = ImGuizmo::OPERATION::ROTATE;
		}
		if (Input::IsKeyClicked(KeyBoardKey::S)) {
			GuizmoType = ImGuizmo::OPERATION::SCALE;
		}
		if(Input::IsKeyPressed(KeyBoardKey::H)){
			//std::stringstream stream;
			//stream << std::hex << UUID();
			//std::string result(stream.str());
			//uint64_t num= Random::Uint<uint64_t>();
			long double num = Random::Int<int>(0,1);

			std::cout<< num <<std::endl;
		}
		m_WorldRenderer.Renderer();
		/*
		glm::mat4 view = -glm::mat4(glm::mat3(ActiveWorld->m_EditorCamera.m_View));
		m_WorldRenderer.m_ScreenFrameBuffer->Bind();
		glDepthFunc(GL_LEQUAL);
		m_SkyBoxShader->Bind();
		m_SkyBoxShader->SetMat4("view",view);
		m_SkyBoxShader->SetMat4("projection", ActiveWorld->m_EditorCamera.m_Projection);
		m_SkyBoxVertexArray->Bind();
		glActiveTexture(GL_TEXTURE0);
		m_CubeMap->Bind();
		glDrawArrays(GL_TRIANGLES,0,36);
		m_SkyBoxVertexArray->UnBind();
		glDepthFunc(GL_LESS);
		RendererCommand::SetClearColor();
		m_WorldRenderer.m_ScreenFrameBuffer->UnBind();
		*/
	}

	void Editore3D::OnImGuiDraw(FrameTime DeltaTime) {
		Layer::OnImGuiDraw(DeltaTime);
		static bool EnableDocking = true;
		SetDocking(&EnableDocking);
		//ImGui::ShowDemoWindow();
		for (auto& a : m_AllPanels) {
			a.second->ImGuiRender(DeltaTime);
		}
		if (ActiveWorld->m_CurrentState == WorldState::Edit)
			ActiveWorld->OnUpdateEditor(DeltaTime,_ViewPortSize.x,_ViewPortSize.y);
		else if (ActiveWorld->m_CurrentState == WorldState::Play)
			ActiveWorld->OnUpdateRuntime(DeltaTime,_ViewPortSize.x,_ViewPortSize.y);
		else if (ActiveWorld->m_CurrentState == WorldState::Simulate)
			ActiveWorld->OnSimulatePhysics(DeltaTime,_ViewPortSize.x,_ViewPortSize.y);

		ViewPort();
		
		MainToolBar();
		
		m_WorldHierachy.ImGuiRender();
		m_CurrentContentBrowserPanel.ImGuiRender();
		Logger();
		if (ImGui::Begin("Renderer Stastitics")) {
			ImGui::TextColored({1.0,0,0,1},"RENDERER SPECS");
			ImGui::Text("Renderer Company: %s",Renderer::GetRenderCompany().c_str());
			ImGui::Text("Graphics Card: %s",Renderer::GetGraphicsCard().c_str());
			ImGui::Text("Graphics Card Verison: %s",Renderer::GetGraphicsCardVersion().c_str());
			ImGui::Text("%.3f ms/frame %.1f FPS",FrameTime::GetFrameMS(),FrameTime::GetFrameFPS());

			ImGui::TextColored({1.0,0,0,1},"RENDERER 3D");
			ImGui::Text("DrawCalls %i",m_WorldRenderer.RenderData.Stats.DrawCalls);
			ImGui::Text("Number Of MeshInstances %i", m_WorldRenderer.RenderData.Stats.Instances);

			ImGui::Text("Amount Of Directional Light %i", m_WorldRenderer.RenderData.Stats.AmountDirectionalLight);
			ImGui::Text("Amount Of Point Light%i", m_WorldRenderer.RenderData.Stats.AmountPointLight);
			ImGui::Text("Amount Of Spot Light %i", m_WorldRenderer.RenderData.Stats.AmountSpotLight);

			if(ImGui::Button("Reload Shader")){
				Renderer::GetShaderLibrary().ReloadeShaders();
			}

			if (m_WorldRenderer.RenderData.RenderSettings.Technique == RenderTechnique::FowardRendering) {
				ImGui::Text("Renderer techniqe is FowardRendering");
			}
			else {
				ImGui::Text("Renderer techniqe is DeferedRendering");

			}
			if (ImGui::Button("Change Renderer")) {
				if (m_WorldRenderer.RenderData.RenderSettings.Technique == RenderTechnique::FowardRendering)
					m_WorldRenderer.RenderData.RenderSettings.Technique = RenderTechnique::DeferedRendering;
				else
					m_WorldRenderer.RenderData.RenderSettings.Technique = RenderTechnique::FowardRendering;
			}
		}
		ImGui::End();

		if (ImGui::Begin("Active World")) {
			if(ImGui::Button("Choose Hdr")){
				std::string file= Utils::FileDialogs::OpenFile("Texture (*.hdr)\0");
				if(file.empty()==false){
					ActiveWorld->CreateIBlTexture(file);
				}
			}
		}
		ImGui::End();
	}
	void Editore3D::Logger() {
		if(ImGui::Begin("Log",(bool*)0,ImGuiWindowFlags_MenuBar| ImGuiWindowFlags_AlwaysHorizontalScrollbar|ImGuiWindowFlags_AlwaysVerticalScrollbar)){
			ImGui::BeginMenuBar();
			{
				ExternalAPI::ImGUIAPI::CheckBox("pause logging",&Log::m_PauseLog);
				ImGui::SameLine();
				if(ImGui::Button("Clear log")){
					Log::Logs.clear();
					ImGui::SetScrollHere();
				}
			}
			ImGui::EndMenuBar();
			int pos=0;
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,1);
			for (auto& it : Log::Logs) {
				if(it.second.first== 0){// ERROR
					ImGui::PushID(pos);
					if (pos % 2 == 0)
						ImGui::PushStyleColor(ImGuiCol_FrameBg,{0.15f,0.15f,0.15f,1.0f});
					ImGui::BeginChildFrame(pos+1,{ImGui::GetWindowWidth(),27});
					ImGui::TextColored({1.0,0.0,0.0,1.0},it.second.second.c_str()); 
					if (ImGui::BeginPopupContextWindow(0,1,false)) {
						if (ImGui::MenuItem("Copy"))
							Utils::ShortCutDialogs::Copy(it.second.second);
						ImGui::EndPopup();
					}
					ImGui::EndChildFrame();
					if (pos % 2 == 0)
						ImGui::PopStyleColor();

					ImGui::PopID();

				}else if (it.second.first ==1){// warn
					ImGui::PushID(pos);
					if (pos % 2 == 0)
						ImGui::PushStyleColor(ImGuiCol_FrameBg,{0.15f,0.15f,0.15f,1.0f});
					ImGui::BeginChildFrame(pos+1,{ImGui::GetWindowWidth(),27});
					ImGui::TextColored({1.0,0.635,0.0,1.0},it.second.second.c_str());
					if (ImGui::BeginPopupContextWindow(0,1,false)) {
						if (ImGui::MenuItem("Copy"))
							Utils::ShortCutDialogs::Copy(it.second.second);
						ImGui::EndPopup();
					}
					ImGui::EndChildFrame();
					if (pos % 2 == 0)
						ImGui::PopStyleColor();

					ImGui::PopID();
				}
				else if( it.second.first ==2){// INFO
					ImGui::PushID(pos);
					if (pos % 2 == 0)
						ImGui::PushStyleColor(ImGuiCol_FrameBg,{0.15f,0.15f,0.15f,1.0f});
					ImGui::BeginChildFrame(pos+1,{ImGui::GetWindowWidth(),27});
					if(ImGui::BeginPopupContextWindow(0,1,false)) { 
						if (ImGui::MenuItem("Copy"))
							Utils::ShortCutDialogs::Copy(it.second.second);
						ImGui::EndPopup();
					}
					ImGui::TextColored({0.0,1.0,0.0,1.0},it.second.second.c_str());
					ImGui::EndChildFrame();
					if (pos % 2 == 0)
						ImGui::PopStyleColor();

					ImGui::PopID();
				}
				else if( it.second.first ==3){ // trace
					ImGui::PushID(pos);
					if (pos % 2 == 0)
						ImGui::PushStyleColor(ImGuiCol_FrameBg,{0.15f,0.15f,0.15f,1.0f});
					ImGui::BeginChildFrame(pos+1,{ImGui::GetWindowWidth(),27});
					if (ImGui::BeginPopupContextWindow(0,1,false)) {
						if (ImGui::MenuItem("Copy"))
							Utils::ShortCutDialogs::Copy(it.second.second);
						ImGui::EndPopup();
					}
					ImGui::TextColored({1.0,1.0,1.0,1.0},it.second.second.c_str());
					ImGui::EndChildFrame();
					if (pos % 2 == 0)
						ImGui::PopStyleColor();

					ImGui::PopID();
				}else{ // CRITITCAL
					ImGui::PushID(pos);

					ImGui::PushStyleColor(ImGuiCol_FrameBg,{1,1,0,1});
					ImGui::BeginChildFrame(pos+1,{ImGui::GetWindowWidth(),27});
					if (ImGui::BeginPopupContextWindow(0,1,false)) {
						if (ImGui::MenuItem("Copy"))
							Utils::ShortCutDialogs::Copy(it.second.second);
						ImGui::EndPopup();
					}
					ImGui::TextColored({1,0,0,1},it.second.second.c_str());
					ImGui::EndChildFrame();

					ImGui::PopStyleColor();
					ImGui::PopID();
				}
				pos+=1;
			}
			ImGui::PopStyleVar();
			if (Log::NewLog == true&& ImGui::IsWindowFocused()==false) {
				ImGui::SetScrollHere();
				Log::NewLog =false;
			}
		
		}
		ImGui::End();
	}
	void Editore3D::ViewPort() {

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0,0});
		static bool Open = true;
		if (ImGui::Begin("ViewPort",&Open)) {

			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();
			ImVec2 ViewPortPanelSize = ImGui::GetWindowSize();
			m_ViewportBounds[0] = {viewportMinRegion.x + viewportOffset.x,viewportMinRegion.y + viewportOffset.y};
			m_ViewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x,viewportMaxRegion.y + viewportOffset.y};
			if (_ViewPortSize != *((glm::vec2*)&ViewPortPanelSize)) {
				_ViewPortSize = {ViewPortPanelSize.x,ViewPortPanelSize.y};
				//m_WorldRenderer.m_ScreenFrameBuffer->Resize(_ViewPortSize.x, _ViewPortSize.y);
				// Still neees to be fixed se
			}

			
			if (ImGui::IsWindowFocused()) {
				CurrentWindow::SetWindowInputEvent(true);
			}
			else {
				CurrentWindow::SetWindowInputEvent(false);
			}
			uint32_t Text = m_WorldRenderer.m_ScreenFrameBuffer->GetTexture();
			ImGui::Image((ImTextureID)Text,ImVec2{_ViewPortSize.x,_ViewPortSize.y},ImVec2{0,1},ImVec2{1,0});
			// GUIZMOS

			Entity selectedEntity = m_WorldHierachy.GetSelectedEntity();

			if (selectedEntity.GetID() != 0) {
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(m_ViewportBounds[0].x,m_ViewportBounds[0].y,m_ViewportBounds[1].x - m_ViewportBounds[0].x,m_ViewportBounds[1].y - m_ViewportBounds[0].y);

				const glm::mat4& cameraProjection = ActiveWorld->m_EditorCamera.m_Projection;
				glm::mat4 cameraView = ActiveWorld->m_EditorCamera.m_View;

				auto& tc = *selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetLocalTransform();
				glm::mat4 worldTransform = tc.GetWorldTransform();
				bool snap = Input::IsKeyPressed(KeyBoardKey::LeftControl);
				float snapValue = 0.5f; // Snap to 0.5m for translation/scale
				// Snap to 45 degrees for rotation
				if (GuizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = {snapValue,snapValue,snapValue};
				if(selectedEntity.GetComponent<SubEntityComponet>()->HasEntityOwner()==false){

					ImGuizmo::Manipulate(glm::value_ptr(cameraView),glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)GuizmoType,ImGuizmo::LOCAL,glm::value_ptr(transform),
					nullptr,snap ? snapValues : nullptr);

					if (ImGuizmo::IsUsing()) {
						glm::vec3 translation,rotation,scale;
						MathResource::DecomposeTransform(transform,translation,rotation,scale);

						glm::vec3 deltaRotation = rotation - glm::vec3{tc.Rotation};
						deltaRotation*=0.5;
						tc.Location = translation;
						tc.Rotation += deltaRotation;
						tc.Scale = scale;
					}
				}else{
					ImGuizmo::Manipulate(glm::value_ptr(cameraView),glm::value_ptr(cameraProjection),
						(ImGuizmo::OPERATION)GuizmoType,ImGuizmo::LOCAL,glm::value_ptr(worldTransform),
							nullptr,snap ? snapValues : nullptr);

					if (ImGuizmo::IsUsing()) {
						glm::vec3 translation,rotation,scale;
						MathResource::DecomposeTransform(worldTransform,translation,rotation,scale);

						glm::vec3 deltaRotation = rotation - glm::vec3{tc.Rotation};
						tc.Location = translation;
						tc.Rotation = deltaRotation;
						tc.Scale = scale;
					}
				}
				
				
			}

			/* putting this underneath image because a window only accpet drop target to when item is bound so and image has been bound */
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("World")) {
					std::string Data = (const char*)payload->Data;
					SceneSerializer scerilize(ActiveWorld.get());
					scerilize.SerilizeText(ActiveWorld->GetPath());
					m_WorldHierachy.m_SelectedEntity = {};

					ActiveWorld = CreateCount<World>();
					SceneSerializer ScerilizerNewWorld(ActiveWorld.get());
					ScerilizerNewWorld.DeSerilizeText(Data);
					m_WorldHierachy.SetContext(ActiveWorld.get());
				}
				ImGui::EndDragDropTarget();
			}
			/*----------------------------------------------------------------------------------------------------------------------------*/
		}
		ImGui::End();
		ImGui::PopStyleVar();
		
		/*
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0,0});
		if (ImGui::Begin("Position")) {
			ImGui::Image((ImTextureID)Renderer3DPBR::GetRenderer()->m_DeferedRendering.GPosition->GetID(),{ImGui::GetWindowSize().x,ImGui::GetWindowSize().y},ImVec2{0,1},ImVec2{1,0});
		}
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0,0});
		if (ImGui::Begin("Colour")) {
			ImGui::Image((ImTextureID)Renderer3DPBR::GetRenderer()->m_DeferedRendering.GAlbedo->GetID(),{ImGui::GetWindowSize().x,ImGui::GetWindowSize().y},ImVec2{0,1},ImVec2{1,0});
		}
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0,0});
		if (ImGui::Begin("Normal")) {
			ImGui::Image((ImTextureID)Renderer3DPBR::GetRenderer()->m_DeferedRendering.GNormal->GetID(),{ImGui::GetWindowSize().x,ImGui::GetWindowSize().y},ImVec2{0,1},ImVec2{1,0});
		}
		ImGui::End();
		ImGui::PopStyleVar();
		*/
	}

	void Editore3D::MainToolBar()
	{
		ImGui::Begin("##MainToolBar",nullptr,ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse/* | ImGuiWindowFlags_NoMove*/);
		Count<Texture2D> icon;
		if( ActiveWorld->m_CurrentState == WorldState::Edit)
			icon = m_PlayButtonTexture;
		else if (ActiveWorld->m_CurrentState==WorldState::Play)
			icon = m_StopButtonTexture;
		else
			icon = m_PlayButtonTexture;

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.4f);
		ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.2f);
		if (ImGui::ImageButton((ImTextureID)icon->GetID(),ImVec2{ImGui::GetWindowSize().y * 0.7f,ImGui::GetWindowSize().y * 0.5f})){
				if(ActiveWorld->m_CurrentState==WorldState::Edit)
					PlayWorld();
				else if(ActiveWorld->m_CurrentState == WorldState::Play)
					SetWorldEdit();
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.5f);
		ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.2f);
		
		if (ImGui::ImageButton((ImTextureID)m_PauseButtonTexture->GetID(),ImVec2{ImGui::GetWindowSize().y * 0.7f,ImGui::GetWindowSize().y * 0.5f})) {
			if(ActiveWorld->m_CurrentState == WorldState::Play)
				PauseWorld();
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.6f);
		ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.2f);

		if (ImGui::ImageButton((ImTextureID)m_SimulateButtonTexture->GetID(),ImVec2{ImGui::GetWindowSize().y * 0.7f,ImGui::GetWindowSize().y * 0.5f})) {
			if (ActiveWorld->m_CurrentState == WorldState::Edit)
				SimulateWorld();
		}
		//ImGui::PopStyleVar(1);
		//ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void Editore3D::SetDocking(bool* p_open) {
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen) {
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize,0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else {
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
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 100.0f; // sets the minimum width of everything

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id,ImVec2(0.0f,0.0f),dockspace_flags);
		}

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New...")) {
					NewWorld(true);
				}
				if (ImGui::MenuItem("Save")) {
					if (ActiveWorld->m_Path.empty() != true) {
						Save();
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	void Editore3D::NewWorld(bool Save) {
		if (Save == true && ActiveWorld != nullptr) {
			SceneSerializer scerelizer(ActiveWorld.get());
			scerelizer.SerilizeText(ActiveWorld->GetPath());
			ActiveWorld = CreateCount<World>();
			m_WorldHierachy.SetContext(ActiveWorld.get()); 
		}
		else {
			ActiveWorld = CreateCount<World>();
			m_WorldHierachy.SetContext(ActiveWorld.get());
			return;
		}
		m_WorldHierachy.SetContext(ActiveWorld.get());
	}

	void Editore3D::OpenWorld() {
		std::string FIle = Utils::FileDialogs::OpenFile("Texture (*.ProofAsset)\0 *.ProofAsset\0 ");
		if (FIle.empty() == false) {
			ActiveWorld = CreateCount<World>();
			SceneSerializer scerelizer(ActiveWorld.get());
			if (scerelizer.DeSerilizeText(FIle) == true) {
				PF_ENGINE_INFO("%s Deserilize perfectly",ActiveWorld->GetName().c_str());
				m_WorldHierachy.SetContext(ActiveWorld.get());
				return;
			}
			else {

				ActiveWorld = CreateCount<World>();
				m_WorldHierachy.SetContext(ActiveWorld.get());
				PF_ENGINE_ERROR("Deceerilize was created default world");
				return;
			}
		}
		PF_ENGINE_TRACE("No File Selected");
		if (ActiveWorld == nullptr) {
			PF_ENGINE_INFO("New World Created cause level was originally nullptr");
			ActiveWorld = CreateCount<World>();
			m_WorldHierachy.SetContext(ActiveWorld.get());
			return;
		}
		PF_ENGINE_INFO("Back to Default WOrld");
	}

	void Editore3D::Save() {
		if (ActiveWorld != nullptr) {
			SceneSerializer scerelizer(ActiveWorld.get());
			scerelizer.SerilizeText(ActiveWorld->GetPath());

			PF_ENGINE_TRACE("%s Saved",ActiveWorld->GetName().c_str());
			return;
		}
		PF_ENGINE_ERROR("World is NULL");
	}
	void Editore3D::PlayWorld() {
		ActiveWorld->m_CurrentState = WorldState::Play;

	}
	void Editore3D::SimulateWorld() {
		ActiveWorld->m_CurrentState = WorldState::Simulate;
	}
	void Editore3D::SetWorldEdit() {
		//ActiveWorld = m_PlayWorld;
		//m_PlayWorld.~shared_ptr();
		ActiveWorld->m_CurrentState = WorldState::Edit;
	}
	void Editore3D::PauseWorld() {
		ActiveWorld->m_CurrentState = WorldState::Pause;
	}
	void Editore3D::Save(const std::string& Path) {
		if (ActiveWorld != nullptr) {
			SceneSerializer scerelizer(ActiveWorld.get());
			scerelizer.SerilizeText(Path);
			PF_ENGINE_TRACE("World Saved");
			return;
		}
		PF_ENGINE_ERROR("World is NULL");
	}


	void Editore3D::CreateMaterialEdtior(MaterialAsset* material) {
		auto it = m_AllPanels.find(material->GetID());
		if (it != m_AllPanels.end()) {
			it->second->SetWindowVisibile(true);
			return;
		}
		MaterialEditorPanel* temp = new MaterialEditorPanel(material);
		m_AllPanels.insert({material->GetID(),temp});
	}

	void Editore3D::CreateMeshEditor(MeshAsset* mesh) {
		auto it =m_AllPanels.find(mesh->GetID());
		if(it!= m_AllPanels.end()){
			it->second->SetWindowVisibile(true);
			return;
		}
		SceneRendererUI* temp = new SceneRendererUI(mesh);
		m_AllPanels.insert({mesh->GetID(),temp});
	}
}
