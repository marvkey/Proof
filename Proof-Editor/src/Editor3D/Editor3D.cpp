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
#include "Proof/Core/Core.h"
#include<GLFW/glfw3.h>
#include "Proof/Resources/Asset/AssetManager.h"
#include <algorithm>
namespace Proof
{
	Editore3D::Editore3D():
		Layer("Editor3D Layer") 	{	}
	Editore3D::~Editore3D() {
	}
	bool Editore3D::IsKeyPressedEditor(KeyBoardKey Key){
		if (glfwGetKey((GLFWwindow*)CurrentWindow::GetWindowAPI(), (int)Key)) {
			return  true;
		}
		return false;
	}
	bool Editore3D::IsKeyClickedEditor(KeyBoardKey Key){
		return std::find(CurrentWindow::GetWindowClass().KeyboardClicked.begin(), CurrentWindow::GetWindowClass().KeyboardClicked.end(), Key) 
			!= 
			CurrentWindow::GetWindowClass().KeyboardClicked.end();
	}
	void Editore3D::OnEvent(Event& e) {

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyClickedEvent>(PF_BIND_FN(Editore3D::OnKeyClicked));
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
		if (scerelizer.DeSerilizeText("content/Levels/Lightest.ProofWorld") == true) {
			m_WorldHierachy.SetContext(ActiveWorld.get());
			AssetManager::NotifyOpenedNewLevel(scerelizer.GetAssetLoadID());
		}
		m_WorldHierachy.SetContext(ActiveWorld.get());
		m_WorldRenderer = WorldRenderer(ActiveWorld, CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight());
		// cannot be setting it to window size and stuff innit
		m_EditorWorld = ActiveWorld;
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
		
		if(Input::IsKeyPressed(KeyBoardKey::H)){
			//std::stringstream stream;
			//stream << std::hex << UUID();
			//std::string result(stream.str());
			//uint64_t num= Random::Uint<uint64_t>();
			long double num = Random::Int<int>(0,1);

			std::cout<< num <<std::endl;
		}
		m_WorldRenderer.Renderer();
		if (ActiveWorld->m_CurrentState == WorldState::Edit)
			ActiveWorld->OnUpdateEditor(DeltaTime, _ViewPortSize.x, _ViewPortSize.y);
		else if (ActiveWorld->m_CurrentState == WorldState::Play)
			ActiveWorld->OnUpdateRuntime(DeltaTime, _ViewPortSize.x, _ViewPortSize.y);
		else if (ActiveWorld->m_CurrentState == WorldState::Simulate)
			ActiveWorld->OnSimulatePhysics(DeltaTime, _ViewPortSize.x, _ViewPortSize.y);
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


		ViewPort();
		
		MainToolBar();
		
		m_WorldHierachy.ImGuiRender(DeltaTime);
		m_CurrentContentBrowserPanel.ImGuiRender(DeltaTime);
		m_AssetManagerPanel.ImGuiRender(DeltaTime);
		Logger();
	
		if (m_ShowWorldEditor == false)
			goto a;
		if (ImGui::Begin("Active World", &m_ShowWorldEditor)) {
			if(ImGui::Button("Choose HDR")){
				std::string file= Utils::FileDialogs::OpenFile("Texture (*.hdr)\0");
				if(file.empty()==false){
					ActiveWorld->CreateIBlTexture(file);
				}
			}
		}
		ImGui::End();
		a:
		if (m_ShowRendererStats == false)
			return;
		if (ImGui::Begin("Renderer Stastitics"), &m_ShowRendererStats) {
			ImGui::TextColored({ 1.0,0,0,1 }, "RENDERER SPECS");
			ImGui::Text("Renderer Company: %s", Renderer::GetRenderCompany().c_str());
			ImGui::Text("Graphics Card: %s", Renderer::GetGraphicsCard().c_str());
			ImGui::Text("Graphics Card Verison: %s", Renderer::GetGraphicsCardVersion().c_str());
			ImGui::Text("%.3f ms/frame %.1f FPS", FrameTime::GetFrameMS(), FrameTime::GetFrameFPS());

			ImGui::TextColored({ 1.0,0,0,1 }, "RENDERER 3D");
			ImGui::Text("DrawCalls %i", m_WorldRenderer.RenderData.Stats.DrawCalls);
			ImGui::Text("Number Of MeshInstances %i", m_WorldRenderer.RenderData.Stats.Instances);

			ImGui::Text("Amount Of Directional Light %i", m_WorldRenderer.RenderData.Stats.AmountDirectionalLight);
			ImGui::Text("Amount Of Point Light%i", m_WorldRenderer.RenderData.Stats.AmountPointLight);
			ImGui::Text("Amount Of Spot Light %i", m_WorldRenderer.RenderData.Stats.AmountSpotLight);

			if (ImGui::Button("Reload Shader")) {
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
	}
	
	void Editore3D::OnKeyClicked(KeyClickedEvent& e) {
		// Shortcuts
		
		bool control = IsKeyPressedEditor(KeyBoardKey::LeftControl) || IsKeyPressedEditor(KeyBoardKey::RightControl);
		bool shift = IsKeyPressedEditor(KeyBoardKey::LeftShift) || IsKeyPressedEditor(KeyBoardKey::RightShift);

		switch (e.GetKey())
		{
		
			case KeyBoardKey::S:
			{
				//if (control && shift)
					//SaveSceneAs();

				break;
			}

			case KeyBoardKey::D:
			{
				if (control && m_WorldHierachy.m_SelectedEntity.GetID()!=0)
					m_WorldHierachy.m_SelectedEntity = ActiveWorld->CreateEntity(m_WorldHierachy.m_SelectedEntity);
				break;

			}
			case KeyBoardKey::Delete:
			case KeyBoardKey::Backspace:
			{
				if (m_WorldHierachy.m_SelectedEntity.GetID() != 0 && (m_ViewPoartHoveredorFocused || m_WorldHierachy.m_WindowHoveredorFocus)) {
					ActiveWorld->DeleteEntity(m_WorldHierachy.m_SelectedEntity);
					m_WorldHierachy.m_SelectedEntity = {};
				}
				break;
			}
			// copy entity
			case KeyBoardKey::C:
			{
				if (control && m_WorldHierachy.m_SelectedEntity.GetID() != 0 &&(m_ViewPoartHoveredorFocused || m_WorldHierachy.m_WindowHoveredorFocus)) {
					m_CopyEntity = m_WorldHierachy.m_SelectedEntity;
				}
				break;
			}
			// paste entity 
			case KeyBoardKey::V:
			{
				if (control && m_CopyEntity.GetID() != 0 && (m_ViewPoartHoveredorFocused || m_WorldHierachy.m_WindowHoveredorFocus)) {
					m_WorldHierachy.m_SelectedEntity = ActiveWorld->CreateEntity(m_CopyEntity);
				}
				break;
			}

			// ImGuizmo
			case KeyBoardKey::Q:
			{
				// no right button pressed that means that we are using the editor camera
				if (m_ViewPoartHoveredorFocused && Input::IsMouseButtonPressed(MouseButton::ButtonRight) == false)
					GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}

			case KeyBoardKey::W:
			{
				// no right button pressed that means that we are using the editor camera
				if (m_ViewPoartHoveredorFocused && Input::IsMouseButtonPressed(MouseButton::ButtonRight) == false)
					GuizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case KeyBoardKey::E:
			{
				// no right button pressed that means that we are using the editor camera
				if (m_ViewPoartHoveredorFocused && Input::IsMouseButtonPressed(MouseButton::ButtonRight) == false)
					GuizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
			case KeyBoardKey::R:
			{
				// no right button pressed that means that we are using the editor camera
				if (m_ViewPoartHoveredorFocused && Input::IsMouseButtonPressed(MouseButton::ButtonRight) == false)
					GuizmoType = ImGuizmo::OPERATION::UNIVERSALV2;
				break;
			}
			case KeyBoardKey::Tab:
			{
				if(m_ViewPortFocused == false || m_WorldHierachy.m_SelectedEntity == false)
					break;
				Entity selected = m_WorldHierachy.m_SelectedEntity;
				if (shift == true) {
					if (selected.HasChildren()) {
						m_WorldHierachy.m_SelectedEntity = { selected.GetComponent<ChildComponent>()->m_Children[0],ActiveWorld.get()};
					}
				}
				
				else if (selected.HasOwner()) {
					int childIndex = selected.GetOwner().GetComponent<ChildComponent>()->GetChildIndex(*selected.GetComponent<ChildComponent>());
					int numChildren = selected.GetOwner().GetComponent<ChildComponent>()->GetNumChildren() - 1;
					int childIndexAdd = 0;
					childIndexAdd += childIndex;
					if (childIndex >= numChildren)
						m_WorldHierachy.m_SelectedEntity = Entity{ selected.GetOwner().GetComponent<ChildComponent>()->m_Children[0],ActiveWorld.get() };
					else if (childIndex <numChildren)						   
						m_WorldHierachy.m_SelectedEntity = Entity{ selected.GetOwner().GetComponent<ChildComponent>()->m_Children[childIndexAdd],ActiveWorld.get() };
				}
				else if (selected.HasChildren()) {
					m_WorldHierachy.m_SelectedEntity = { selected.GetComponent<ChildComponent>()->m_Children[0],ActiveWorld.get() };
				}
				break;
			}
		}
	}
	void Editore3D::Logger() {
		if (m_ShowLogger == false)
			return;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		if(ImGui::Begin("Log", &m_ShowLogger,ImGuiWindowFlags_MenuBar| ImGuiWindowFlags_AlwaysHorizontalScrollbar|ImGuiWindowFlags_AlwaysVerticalScrollbar)){
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
		ImGui::PopStyleVar();
	}
	void Editore3D::ViewPort() {

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0,0});
		static bool Open = true;
		if (ImGui::Begin("ViewPort",&Open,ImGuiWindowFlags_NoScrollWithMouse| ImGuiWindowFlags_NoScrollbar)) {
			m_ViewPoartHoveredorFocused = ImGui::IsWindowHovered() ||ImGui::IsWindowFocused();
			m_ViewPortFocused = ImGui::IsWindowFocused();
			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();
			ImVec2 ViewPortPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x,viewportMinRegion.y + viewportOffset.y };
			m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x,viewportMaxRegion.y + viewportOffset.y };
			if (_ViewPortSize != *((glm::vec2*)&ViewPortPanelSize)) {
				_ViewPortSize = { ViewPortPanelSize.x,ViewPortPanelSize.y };
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
				ImGuizmo::SetOrthographic(true);
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

				const glm::mat4& cameraProjection = ActiveWorld->m_EditorCamera.m_Projection;
				glm::mat4 cameraView = ActiveWorld->m_EditorCamera.m_View;

				auto& tc = *selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetWorldTransform();

				bool snap = Input::IsKeyPressed(KeyBoardKey::LeftControl);
				float snapValue = 0.5f; // Snap to 0.5m for translation/scale
				// Snap to 45 degrees for rotation
				if (GuizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue,snapValue,snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)GuizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
					nullptr, snap ? snapValues : nullptr);;

				if (ImGuizmo::IsUsing()) {
					
					if (selectedEntity.HasOwner() == false) {
						glm::vec3 translation, rotation, scale;
						MathResource::DecomposeTransform(transform, translation, rotation, scale);
						glm::vec3 deltaRotation = glm::vec3{ glm::degrees(rotation.x),glm::degrees(rotation.y),glm::degrees(rotation.z) } - glm::vec3{ tc.Rotation };
						if(GuizmoType == ImGuizmo::OPERATION::TRANSLATE)
							tc.Location = translation;
						else if(GuizmoType == ImGuizmo::OPERATION::ROTATE)
							tc.Rotation += deltaRotation;
						else
						tc.Scale = scale;
					}
					else {
						glm::vec3 translation, rotation, scale;
						MathResource::DecomposeTransform(transform, translation, rotation, scale);
						tc.Location = tc.GetWorldTransform() * glm::vec4(translation,1.0);
						glm::vec3 tempScale = tc.Scale;
						//glm::vec3 deltaRotation = rotation - glm::vec3{ tc.Rotation };
						//tc.Location = translation - tempLocation;
						//tc.Rotation += deltaRotation;
						//tc.Scale = glm::vec3{ tc.GetWorldScale() }- scale;
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

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(MeshAsset::GetAssetType().c_str())) {
					UUID meshID = *(UUID*)payload->Data;

					Entity newentt = ActiveWorld->CreateEntity(AssetManager::GetAssetName(meshID));
					newentt.AddComponent<MeshComponent>()->SetMeshSource(meshID);
					m_WorldHierachy.m_SelectedEntity = newentt;
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
			if (ImGui::BeginMenu("Edit")) {

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View")) {
				ImGui::MenuItem("Heirachy", nullptr, &m_WorldHierachy.m_ShowWindow);
				ImGui::MenuItem("Log", nullptr, &m_ShowLogger);
				ImGui::MenuItem("Content Browser", nullptr, &m_CurrentContentBrowserPanel.m_ShowWindow);
				ImGui::MenuItem("Asset Manager ", nullptr, &m_AssetManagerPanel.m_ShowWindow);
				ImGui::MenuItem("Render Stats", nullptr, &m_ShowRendererStats);
				ImGui::MenuItem("World Editor", nullptr, &m_ShowWorldEditor);
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
		ActiveWorld = World::Copy(m_EditorWorld);
		ActiveWorld->m_CurrentState = WorldState::Play;
		m_WorldHierachy.SetContext(ActiveWorld.get());
		m_WorldRenderer.SetContext(ActiveWorld);
		ActiveWorld->StartRuntime();
	}
	void Editore3D::SimulateWorld() {
		GuizmoType = 0;
		ActiveWorld->m_CurrentState = WorldState::Simulate;
	}
	void Editore3D::SetWorldEdit() {

		GuizmoType = 0;
		ActiveWorld->EndRuntime();
		ActiveWorld = m_EditorWorld;
		m_WorldHierachy.SetContext(ActiveWorld.get());
		m_WorldRenderer.SetContext(ActiveWorld);
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
		if (material == nullptr)return;
		auto it = m_AllPanels.find(material->GetID());
		if (it != m_AllPanels.end()) {
			it->second->SetWindowVisibile(true);
			return;
		}
		MaterialEditorPanel* temp = new MaterialEditorPanel(material);
		m_AllPanels.insert({material->GetID(),temp});
	}

	void Editore3D::CreateMeshEditor(MeshAsset* mesh) {
		if (mesh == nullptr)return;
		auto it =m_AllPanels.find(mesh->GetID());
		if(it!= m_AllPanels.end()){
			it->second->SetWindowVisibile(true);
			return;
		}
		SceneRendererUI* temp = new SceneRendererUI(mesh);
		m_AllPanels.insert({mesh->GetID(),temp});
	}
}
