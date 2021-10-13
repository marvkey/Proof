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


namespace Proof
{
	glm::vec4 ClearColour;

	Editore3D::Editore3D():
		Layer("Editor3D Layer") 	{	}
	Editore3D::~Editore3D() {
		if (ActiveWorld != nullptr)
			delete ActiveWorld;
	}
	void Editore3D::OnAttach() {

		ActiveWorld = new World();
		m_WorldHierachy.SetContext(ActiveWorld);
		m_WorldHierachy.SetBrowserPanel(&m_CurrentContentBrowserPanel);
		CubeMapPaths.emplace_back("Assets/Textures/skybox/right.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/left.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/bottom.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/top.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/front.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/back.jpg");
		
		
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

		m_SkyBoxShader = Shader::Create("SkyBox Shader",ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/CubeMapShader.shader");
		m_SkyBoxBuffer = VertexBuffer::Create(&skyboxVertices,sizeof(skyboxVertices));
		m_SkyBoxVertexArray = VertexArray::Create();
		m_SkyBoxVertexArray->AddData(0,3,3 * sizeof(float),(void*)0);

		m_SkyBoxShader->Bind();
		m_SkyBoxShader->SetInt("skybox",0);


		m_CubeMap = CubeMap::Create(CubeMapPaths);

		m_PlayButtonTexture = Texture2D::Create("Resources/Icons/MainPanel/PlayButton.png");
		m_PauseButtonTexture = Texture2D::Create("Resources/Icons/MainPanel/PauseButton .png");
		m_SimulateButtonTexture = Texture2D::Create("Resources/Icons/MainPanel/SimulateButton.png");
		m_StopButtonTexture =Texture2D::Create("Resources/Icons/MainPanel/StopButton.png");

		//PF_ENGINE_INFO("number ofscript is %i",ScriptDetail::GetScriptRegisry().size());
	}
	void Editore3D::OnDetach() {
		if (ActiveWorld != nullptr) {
			SceneSerializer scerelizer(ActiveWorld);
			scerelizer.SerilizeText(ActiveWorld->GetPath());
		}
	}

	void Editore3D::OnUpdate(FrameTime DeltaTime) {
		Layer::OnUpdate(DeltaTime);
		Application::ViewPortWidth = _ViewPortSize.x;
		Application::ViewPortHeight = _ViewPortSize.y;
		if (Input::IsKeyClicked(KeyBoardKey::L)) {
			GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
		}
		if (Input::IsKeyClicked(KeyBoardKey::R)) {
			GuizmoType = ImGuizmo::OPERATION::ROTATE;
		}
		if (Input::IsKeyClicked(KeyBoardKey::S)) {
			GuizmoType = ImGuizmo::OPERATION::SCALE;
		}

		if(Input::IsKeyPressed(KeyBoardKey::L)){
			PF_ENGINE_TRACE("THE LETTER  IS PRESSED ");
		}
		if(ActiveWorld->m_CurrentState == WorldState::Edit)
			ActiveWorld->OnUpdateEditor(DeltaTime);
		else if(ActiveWorld->m_CurrentState == WorldState::Play)
			ActiveWorld->OnUpdateRuntime(DeltaTime);
		else if(ActiveWorld->m_CurrentState== WorldState::Simulate)
			ActiveWorld->OnSimulatePhysics(DeltaTime);
		glm::mat4 view = -glm::mat4(glm::mat3(ActiveWorld->EditorCamera.GetCameraView())); /// makes makes the sky box move around player, makes it seem the sky box is very large

		/*
		glDepthFunc(GL_LEQUAL);
		m_SkyBoxShader->UseShader();
		m_SkyBoxShader->SetMat4("view",view);
		m_SkyBoxShader->SetMat4("projection",ActiveWorld->Projection);

		m_SkyBoxVertexArray->Bind();
		glActiveTexture(GL_TEXTURE0);
		m_CubeMap->Bind();
		glDrawArrays(GL_TRIANGLES,0,36);
		m_SkyBoxVertexArray->UnBind();
		glDepthFunc(GL_LESS);
		*/
		//RendererCommand::SetClearColor(ClearColour);

	}

	void Editore3D::OnImGuiDraw() {
		Layer::OnImGuiDraw();
		static bool EnableDocking = true;
		SetDocking(&EnableDocking);
		//ImGui::ShowDemoWindow();
		ViewPort();
		MainToolBar();
		m_WorldHierachy.ImGuiRender();
		m_CurrentContentBrowserPanel.ImGuiRender();
		MaterialEditor();
		Logger();
		if (ImGui::Begin("Renderer Stastitics")) {
			ImGui::TextColored({1.0,0,0,1},"RENDERER SPECS");
			ImGui::Text("Renderer Company: %s",Renderer::GetRenderCompany().c_str());
			ImGui::Text("Graphics Card: %s",Renderer::GetGraphicsCard().c_str());
			ImGui::Text("Graphics Card Verison: %s",Renderer::GetGraphicsCardVersion().c_str());
			ImGui::Text("%.3f ms/frame %.1f FPS",FrameTime::GetFrameMS(),FrameTime::GetFrameFPS());

			ImGui::TextColored({1.0,0,0,1},"RENDERER 3D");
			ImGui::Text("DrawCalls %i",Renderer3D::Render3DStats::DrawCalls);
			ImGui::Text("Number Of MeshInstances %i",Renderer3D::Render3DStats::NumberOfInstances);
			ImGui::Text("Amount Of Meshes Drawn %i",Renderer3D::Render3DStats::AmountDrawn);

			ImGui::TextColored({1.0,0,0,1},"RENDERER 2D");
			ImGui::Text("DrawCalls %i",Renderer2D::Renderer2DStats::m_DrawCalls);
			ImGui::Text("Number of Quads %i",Renderer2D::Renderer2DStats::m_QuadCount);
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
					ImGui::TextColored({1.0,1.0,1.0,1.0},it.second.second.c_str());
					ImGui::EndChildFrame();
					if (pos % 2 == 0)
						ImGui::PopStyleColor();

					ImGui::PopID();
				}else{ // CRITITCAL
					ImGui::PushID(pos);

					ImGui::PushStyleColor(ImGuiCol_FrameBg,{1,1,0,1});
					ImGui::BeginChildFrame(pos+1,{ImGui::GetWindowWidth(),27});
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
		if (ImGui::Begin("ViewPort",&Open,ImGuiWindowFlags_NoMove)) {
			ImVec2 ViewPortPanelSize = ImGui::GetContentRegionAvail();
			if (_ViewPortSize != *((glm::vec2*)&ViewPortPanelSize)) {
				_ViewPortSize = {ViewPortPanelSize.x,ViewPortPanelSize.y};
			}

			if (ImGui::IsWindowFocused()) {
				Input::ViewPoartHovered = true;
			}
			else {
				Input::ViewPoartHovered = false;
			}

			uint32_t Text = Application::GetScreenBuffer()->GetTexture();
			ImGui::Image((ImTextureID)Text,ImVec2{_ViewPortSize.x,_ViewPortSize.y},ImVec2{0,1},ImVec2{1,0});
			// GUIZMOS

			Entity selectedEntity = this->m_WorldHierachy.GetSelectedEntity();

			if (selectedEntity.GetID() != 0) {
				/*
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				float windowWidth  =(float) ImGui::GetWindowWidth();
				float windowHeight = (float) ImGui::GetWindowHeight();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x,ImGui::GetWindowPos().y,windowWidth,windowHeight);

				const glm::mat4& cameraProjection = ActiveWorld->Projection;
				glm::mat4 cameraView = glm::inverse(ActiveWorld->EditorCamera.GetTransform());


				auto& tc = *selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetTransform();

				ImGuizmo::Manipulate(glm::value_ptr(cameraView),glm::value_ptr(cameraProjection),
					ImGuizmo::OPERATION(GuizmoType),ImGuizmo::LOCAL,glm::value_ptr(transform));

				if(ImGuizmo::IsUsing()&& GuizmoType != -1){
					glm::vec3  translation,rotation,scale;
					MathResource::DecomposeTransform(transform,translation,rotation,scale);

					glm::vec3 deltaRotaiton = Vector(rotation) -tc.Rotation; // prevent gimble

					tc.Location = translation;
					tc.Rotation+=deltaRotaiton;
					tc.Scale =scale;
				}
				*/
			}

			/* putting this underneath image because a window only accpet drop target to when item is bound so and image has been bound */
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("World")) {
					std::string Data = (const char*)payload->Data;
					SceneSerializer scerilize(ActiveWorld);
					scerilize.SerilizeText(ActiveWorld->GetPath());
					m_WorldHierachy.m_SelectedEntity = {};
					delete ActiveWorld;

					ActiveWorld = new World();
					SceneSerializer ScerilizerNewWorld(ActiveWorld);
					ScerilizerNewWorld.DeSerilizeText(Data);
					m_WorldHierachy.SetContext(ActiveWorld);
				}
				ImGui::EndDragDropTarget();
			}
			/*----------------------------------------------------------------------------------------------------------------------------*/
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Editore3D::MainToolBar()
	{
		ImGui::Begin("##MainToolBar",nullptr,ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse| ImGuiWindowFlags_NoMove);
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
					StopWorld();
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
			SceneSerializer scerelizer(ActiveWorld);
			scerelizer.SerilizeText(ActiveWorld->GetPath());
			delete ActiveWorld;
			ActiveWorld = new World();
			m_WorldHierachy.SetContext(ActiveWorld); 
		}
		else {
			if (ActiveWorld != nullptr) {
				delete ActiveWorld;
			}
			ActiveWorld = new World();
			m_WorldHierachy.SetContext(ActiveWorld);
			return;
		}
		ActiveWorld = new World();
		m_WorldHierachy.SetContext(ActiveWorld);
	}

	void Editore3D::OpenWorld() {
		std::string FIle = Utils::FileDialogs::OpenFile("Texture (*.ProofAsset)\0 *.ProofAsset\0 ");
		if (FIle.empty() == false) {
			ActiveWorld = new World();
			SceneSerializer scerelizer(ActiveWorld);
			if (scerelizer.DeSerilizeText(FIle) == true) {
				PF_ENGINE_INFO("%s Deserilize perfectly",ActiveWorld->GetName().c_str());
				m_WorldHierachy.SetContext(ActiveWorld);
				return;
			}
			else {
				ActiveWorld = new World();
				m_WorldHierachy.SetContext(ActiveWorld);
				PF_ENGINE_ERROR("Deceerilize was created default world");
				return;
			}
		}
		PF_ENGINE_TRACE("No File Selected");
		if (ActiveWorld == nullptr) {
			PF_ENGINE_INFO("New World Created cause level was originally nullptr");
			ActiveWorld = new World();
			m_WorldHierachy.SetContext(ActiveWorld);
			return;
		}
		PF_ENGINE_INFO("Back to Default WOrld");
	}

	void Editore3D::Save() {
		if (ActiveWorld != nullptr) {
			SceneSerializer scerelizer(ActiveWorld);
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
	void Editore3D::StopWorld() {
		ActiveWorld->m_CurrentState = WorldState::Edit;
	}
	void Editore3D::PauseWorld() {
		ActiveWorld->m_CurrentState = WorldState::Pause;
	}
	void Editore3D::Save(const std::string& Path) {
		if (ActiveWorld != nullptr) {
			SceneSerializer scerelizer(ActiveWorld);
			scerelizer.SerilizeText(Path);
			PF_ENGINE_TRACE("World Saved");
			return;
		}
		PF_ENGINE_ERROR("World is NULL");
	}


	void Editore3D::SetMaterialEditor(MaterialAsset& material) {
		mat = &material;
		TempID = material.GetID();
		PF_ENGINE_INFO("material ID %i",TempID);
	}

	void Editore3D::MaterialEditor() {
		ImGui::Begin("Material Editor");
		{
			if (TempID == 0 || AssetManager::HasID(TempID) == false) {
				mat = nullptr;
				TempID = 0;
			}
			else {
				//ImGui::ColorEdit3("Ambient",glm::value_ptr(mat->m_Material.m_Ambient));
				//ImGui::ColorEdit3("Diffuse",glm::value_ptr(mat->m_Material.m_Diuffuse));
				//ImGui::ColorEdit3("Specular",glm::value_ptr(mat->m_Material.m_Specular));
				ImGui::ColorEdit3("Colour",glm::value_ptr(mat->m_Material.m_Colour));
				uint32_t whiteColourId= PhysicalBasedRenderer::m_WhiteTexture->GetID();
				
				ImGui::Image((ImTextureID)(mat->m_Material.AlbedoTexture == nullptr ? whiteColourId : mat->m_Material.AlbedoTexture->GetID()),{50,50});
				if (ImGui::Button("Albedo",{100,50})) {
					std::string file = Utils::FileDialogs::OpenFile("Texture(*.png)\0 * .png\0 (*.jpg)\0 * .jpg\0");
					if (file.empty() == false) {
						mat->m_Material.AlbedoTexture = Texture2D::Create(file);
					}
				}
				ImGui::NewLine();


				ImGui::Image((ImTextureID) (mat->m_Material.MetallicTexture == nullptr ? whiteColourId: mat->m_Material.MetallicTexture->GetID()),{50,50});
				if(ImGui::Button("Metallness",{100,50})){
					std::string file = Utils::FileDialogs::OpenFile("Texture(*.png)\0 * .png\0 (*.jpg)\0 * .jpg\0");
					if(file.empty()==false){
						mat->m_Material.MetallicTexture = Texture2D::Create(file);
					}
				}
				ImGui::NewLine();
				ImGui::DragFloat("Metallnes",&mat->m_Material.m_Metallness,0.001);

				ImGui::Image((ImTextureID)(mat->m_Material.RoughnessTexture == nullptr ? whiteColourId : mat->m_Material.RoughnessTexture->GetID()),{50,50});
				if (ImGui::Button("RoughnessTexture",{100,50})) {
					std::string file = Utils::FileDialogs::OpenFile("Texture(*.png)\0 * .png\0 (*.jpg)\0 * .jpg\0");
					if (file.empty() == false) {
						mat->m_Material.RoughnessTexture = Texture2D::Create(file);
					}
				}
				ImGui::NewLine();
				ImGui::DragFloat("Roughness",&mat->m_Material.m_Roughness,0.001);


				if (ImGui::Button("Normal",{100,50})) {
					std::string file = Utils::FileDialogs::OpenFile("Texture(*.png)\0 * .png\0 (*.jpg)\0 * .jpg\0");
					if (file.empty() == false) {
						mat->m_Material.NormalTexture = Texture2D::Create(file);
					}
				}
				ImGui::Image((ImTextureID)(mat->m_Material.NormalTexture == nullptr ? whiteColourId : mat->m_Material.NormalTexture->GetID()),{50,50});

				ImGui::NewLine();
				ImGui::DragFloat("AO",&mat->m_Material.m_AO,0.001);

				mat->SaveAsset();
			}
		}
		ImGui::End();
	}
}
