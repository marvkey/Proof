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
#include "Proof/Resources/Asset/PhysicsMaterialAsset.h"
#include "Proof/Resources/Asset/MaterialAsset.h"
#include "windows.h"
#include "Proof/Scene/Script.h"

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
#include "Proof/Input/InputManager.h"
#include<thread>
#include <chrono>
#include "Proof/Core/SceneCoreClasses.h"
#include "Platform/Vulkan/VulkanRenderer/VulkanRenderer.h"




#include "Proof/Scripting/ScriptEngine.h"
namespace Proof
{
	EditorCamera camera{ 200,200};

	Editore3D::Editore3D() :
		Layer("Editor3D Layer") {
	}
	Editore3D::~Editore3D() {
	}
	bool Editore3D::IsKeyPressedEditor(KeyBoardKey Key) {
		if (glfwGetKey((GLFWwindow*)CurrentWindow::GetWindowAPI(), (int)Key)) {
			return  true;
		}
		return false;
	}
	bool Editore3D::IsKeyClickedEditor(KeyBoardKey Key) {
		return std::find(CurrentWindow::GetWindowClass().KeyboardClicked.begin(), CurrentWindow::GetWindowClass().KeyboardClicked.end(), Key)
			!=
			CurrentWindow::GetWindowClass().KeyboardClicked.end();
	}
	void Editore3D::OnEvent(Event& e) {
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan)return;
		EventDispatcher dispatcher(e);
		if (ActiveWorld->m_CurrentState == WorldState::Play)
			InputManager::OnEvent(e);

		dispatcher.Dispatch<KeyClickedEvent>(PF_BIND_FN(Editore3D::OnKeyClicked));
		dispatcher.Dispatch<ControllerConnectEvent>([](auto& e) {
			PF_INFO(e.ToString());
		});
		dispatcher.Dispatch<ControllerDisconnectEvent>([](auto& e) {
			PF_INFO(e.ToString());
		});

		// KEYBOARD
		{
			if (m_ShowAllKeyBoardEvents.ShowOne == true && e.IsInCategory(EventCategory::EventKeyBoard)) {
				if (m_ShowAllKeyBoardEvents.ShowAll == true) {
					PF_INFO(e.ToString());
				}
				if (m_ShowAllKeyBoardEvents.Clicked) {
					dispatcher.Dispatch<KeyClickedEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}

				if (m_ShowAllKeyBoardEvents.Released) {
					dispatcher.Dispatch<KeyReleasedEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}

				if (m_ShowAllKeyBoardEvents.DoubleClicked) {
					dispatcher.Dispatch<KeyDoubleClickEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}

				if (m_ShowAllKeyBoardEvents.Pressed) {
					dispatcher.Dispatch<KeyPressedEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}
				return;
			}
		}

		// MOUSE
		{
			if (m_ShowAllMouseEvents.ShowOne == true && e.IsInCategory(EventCategory::EventMouse)) {
				if (m_ShowAllMouseEvents.ShowAll == true) {
					PF_INFO(e.ToString());
					return;
				}
				if (m_ShowAllMouseEvents.Movement) {
					dispatcher.Dispatch<MouseMoveEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}
				if (m_ShowAllMouseEvents.Clicked) {
					dispatcher.Dispatch<MouseButtonClickedEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}

				if (m_ShowAllMouseEvents.Released) {
					dispatcher.Dispatch<MouseButtonReleasedEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}

				if (m_ShowAllMouseEvents.DoubleClicked) {
					dispatcher.Dispatch<MouseButtonDoubleClickEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}

				if (m_ShowAllMouseEvents.Pressed) {
					dispatcher.Dispatch<MouseButtonPressedEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}

				if (m_ShowAllMouseEvents.Scroll) {
					dispatcher.Dispatch<MouseScrollEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}
				return;
			}
		}

		// WINDOW
		{
			if (m_ShowAllWindowEvents.ShowOne == true && e.IsInCategory(EventCategory::EventCategoryWindow)) {
				if (m_ShowAllWindowEvents.ShowAll == true) {
					PF_INFO(e.ToString());
					return;
				}
				if (m_ShowAllWindowEvents.Resize) {
					dispatcher.Dispatch<WindowResizeEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}
				if (m_ShowAllWindowEvents.Minimize) {
					dispatcher.Dispatch<WindowMinimizeEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}

				if (m_ShowAllWindowEvents.Move) {
					dispatcher.Dispatch<WindowMoveEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}

				if (m_ShowAllWindowEvents.Close) {
					dispatcher.Dispatch<WindowCloseEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}

				if (m_ShowAllWindowEvents.Focus) {
					dispatcher.Dispatch<WindowFocusEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}
				return;
			}
		}

		// Controller
		{
			if (m_ShowAllControllerEvents.ShowOne == true && e.IsInCategory(EventCategory::EventController)) {
				if (m_ShowAllMouseEvents.ShowAll == true) {
					PF_INFO(e.ToString());
					return;
				}
				if (m_ShowAllControllerEvents.Clicked) {
					dispatcher.Dispatch<ControllerButtonClickedEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}
				if (m_ShowAllControllerEvents.Released) {
					dispatcher.Dispatch<ControllerButtonReleasedEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}

				if (m_ShowAllControllerEvents.Pressed) {
					dispatcher.Dispatch<ControllerButtonPressedEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}

				if (m_ShowAllControllerEvents.DoubleClicked) {
					dispatcher.Dispatch<ControllerButtonDoubleClickEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}

				if (m_ShowAllControllerEvents.Joystick) {
					dispatcher.Dispatch<ControllerLeftJoystickAxisEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
					dispatcher.Dispatch<ControllerRightJoystickAxisEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}

				if (m_ShowAllControllerEvents.Trigger) {
					dispatcher.Dispatch<ControllerTriggerAxisEvent>([](auto& e) {
						PF_INFO(e.ToString());
					});
				}
				return;
			}
		}
	}
	void Editore3D::OnAttach() {
		camera = EditorCamera{ CurrentWindow::GetWindowHeight(),CurrentWindow::GetWindowWidth() };
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan)return;
		m_CheckeboardTexture = Texture2D::Create("Assets/Textures/CheckeboardTexture.jpg");


		CubeMapPaths.emplace_back("Assets/Textures/skybox/right.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/left.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/bottom.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/top.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/front.jpg");
		CubeMapPaths.emplace_back("Assets/Textures/skybox/back.jpg");

		ActiveWorld = CreateCount<World>();
		//ScriptEngine::ReloadAssembly(ActiveWorld.get());
		SceneSerializer scerelizer(ActiveWorld.get());
		if (scerelizer.DeSerilizeText("content/Levels/Lightest.ProofWorld") == true) {
			m_WorldHierachy.SetContext(ActiveWorld.get());
			if (Renderer::GetAPI() != RendererAPI::API::Vulkan)
				AssetManager::NotifyOpenedNewLevel(scerelizer.GetAssetLoadID());
		}

		m_WorldHierachy.SetContext(ActiveWorld.get());
		m_WorldRenderer = WorldRenderer(ActiveWorld, CurrentWindow::GetWindowWidth(), CurrentWindow::GetWindowHeight());
		// cannot be setting it to window size and stuff innit
		m_EditorWorld = ActiveWorld;
		SceneCoreClasses::s_CurrentWorld = ActiveWorld.get();

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

		m_SkyBoxShader = Shader::GetOrCreate("SkyBox Shader", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/3D/CubeMapShader.shader");
		m_SkyBoxBuffer = VertexBuffer::Create(&skyboxVertices, sizeof(skyboxVertices));
		m_SkyBoxVertexArray = VertexArray::Create();
		m_SkyBoxVertexArray->AddData(0, 3, 3 * sizeof(float), 0);

		m_SkyBoxShader->Bind();
		m_SkyBoxShader->SetInt("skybox", 0);
		m_CubeMap = CubeMap::Create(CubeMapPaths);

		m_PlayButtonTexture = Texture2D::Create("Resources/Icons/MainPanel/PlayButton.png");
		m_PauseButtonTexture = Texture2D::Create("Resources/Icons/MainPanel/PauseButton .png");
		m_SimulateButtonTexture = Texture2D::Create("Resources/Icons/MainPanel/SimulateButton.png");
		m_StopButtonTexture = Texture2D::Create("Resources/Icons/MainPanel/StopButton.png");

	}
	void Editore3D::OnDetach() {
		if (m_EditorWorld != nullptr) { // using editor world in case active world is on play
			SceneSerializer scerelizer(m_EditorWorld.get());
			scerelizer.SerilizeText(m_EditorWorld->GetPath());
		}
	}

	void Editore3D::OnUpdate(FrameTime DeltaTime) {
		PF_PROFILE_FUNC();
		Layer::OnUpdate(DeltaTime);
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan) {
			Count<ScreenFrameBuffer> bufferl;
			camera.OnUpdate(DeltaTime, CurrentWindow::GetWindowWidth(), CurrentWindow::GetWindowHeight());
			RendererData rednerdata;
			VulkanRenderer::BeginContext(camera.m_Projection, camera.m_View, camera.m_Positon, bufferl, rednerdata);
			VulkanRenderer::EndContext();
		}
		if (Renderer::GetAPI() == RendererAPI::API::Vulkan)return;
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
		PF_PROFILE_FUNC();

		Layer::OnImGuiDraw(DeltaTime);
		//ImGui::ShowDemoWindow();
		static bool EnableDocking = true;
		SetDocking(&EnableDocking);
		for (auto& a : m_AllPanels) {
			a.second->ImGuiRender(DeltaTime);
		}

		ViewPort();
		MainToolBar();
		m_WorldHierachy.ImGuiRender(DeltaTime);
		m_ContentBrowserPanel.ImGuiRender(DeltaTime);
		m_AssetManagerPanel.ImGuiRender(DeltaTime);
		m_InputPanel.ImGuiRender(DeltaTime);
		m_PerformancePanel.ImGuiRender(DeltaTime);
		Logger();

		if (m_ShowWorldEditor == false)
			goto a;
		if (ImGui::Begin("Active World", &m_ShowWorldEditor)) {
			if (ImGui::Button("Choose HDR")) {
				std::string file = Utils::FileDialogs::OpenFile("Texture (*.hdr)\0");
				if (file.empty() == false) {
					ActiveWorld->CreateIBlTexture(file);
				}
			}
		}
		ImGui::End();
		a:
		if (m_ShowRendererStats == false)
			return;
		ImGui::Begin("Renderer Stastitics", &m_ShowRendererStats);
		{

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

		switch (e.GetKey()) {

			case KeyBoardKey::S:
				{
					if (control)
						Save();

					break;
				}

			case KeyBoardKey::D:
				{
					if (control && m_WorldHierachy.m_SelectedEntity.GetEntityID() != 0)
						m_WorldHierachy.m_SelectedEntity = ActiveWorld->CreateEntity(m_WorldHierachy.m_SelectedEntity);
					break;

				}
			case KeyBoardKey::Delete:
			case KeyBoardKey::Backspace:
				{
					if (m_WorldHierachy.m_SelectedEntity.GetEntityID() != 0 && (m_ViewPoartHoveredorFocused || m_WorldHierachy.m_WindowHoveredorFocus)) {

						if (ActiveWorld->GetState() == WorldState::Edit) {
							//Basically makig sure that all entities that reference this entity that is deleted their data get sets to null
							ActiveWorld->ForEachEntitiesWithSingle<ScriptComponent>([&](Entity& entity) {
								auto& scp = *entity.GetComponent<ScriptComponent>();
								for (auto& scripts : scp.m_Scripts) {
									for (auto& field : scripts.Fields) {
										if (field.Type == ProofMonoType::Enum) {
											uint64_t* data = field.Data._Cast<uint64_t>();
											if (data == nullptr)
												return;
											if (*data == m_WorldHierachy.m_SelectedEntity.GetEntityID())
												*data = 0;
										}
									}
								}
							});
						}
						ActiveWorld->DeleteEntity(m_WorldHierachy.m_SelectedEntity);
						m_WorldHierachy.m_SelectedEntity = {};
					}
					break;
				}
				// copy entity
			case KeyBoardKey::C:
				{
					if (control && m_WorldHierachy.m_SelectedEntity.GetEntityID() != 0 && (m_ViewPoartHoveredorFocused || m_WorldHierachy.m_WindowHoveredorFocus)) {
						m_CopyEntity = m_WorldHierachy.m_SelectedEntity;
					}
					break;
				}
				// paste entity 
			case KeyBoardKey::V:
				{
					if (control && m_CopyEntity.GetEntityID() != 0 && (m_ViewPoartHoveredorFocused || m_WorldHierachy.m_WindowHoveredorFocus)) {
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
					if (m_ViewPortFocused == false || m_WorldHierachy.m_SelectedEntity == false)
						break;
					Entity selected = m_WorldHierachy.m_SelectedEntity;
					if (shift == true) {
						if (selected.HasChildren()) {
							m_WorldHierachy.m_SelectedEntity = { selected.GetComponent<ChildComponent>()->m_Children[0],ActiveWorld.get() };
						}
					}

					else if (selected.HasOwner()) {
						int childIndex = selected.GetOwner().GetComponent<ChildComponent>()->GetChildIndex(*selected.GetComponent<ChildComponent>());
						int numChildren = selected.GetOwner().GetComponent<ChildComponent>()->GetNumChildren() - 1;
						int childIndexAdd = 0;
						childIndexAdd += childIndex;
						if (childIndex >= numChildren)
							m_WorldHierachy.m_SelectedEntity = Entity{ selected.GetOwner().GetComponent<ChildComponent>()->m_Children[0],ActiveWorld.get() };
						else if (childIndex < numChildren)
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
		if (ImGui::Begin("Log", &m_ShowLogger, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
			ImGui::BeginMenuBar();
			{
				ExternalAPI::ImGUIAPI::CheckBox("pause logging", &Log::m_PauseLog);
				ExternalAPI::ImGUIAPI::CheckBox("Clear On Play", &m_ClearLogOnPlay);
				ImGui::SameLine();
				if (ImGui::Button("Clear log")) {
					Log::Logs.clear();
					ImGui::SetScrollHere();
				}
				if (ImGui::Button("Settings")) {

					Math::ChangeBool(m_ShowLogSettings);
				}
			}
			ImGui::EndMenuBar();
			int pos = 0;
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1);
			for (auto& it : Log::Logs) {
				if (it.second.first == 0) {// ERROR
					ImGui::PushID(pos);
					if (pos % 2 == 0)
						ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.15f,0.15f,0.15f,1.0f });
					ImGui::BeginChildFrame(pos + 1, { ImGui::GetWindowWidth(),27 });
					ImGui::TextColored({ 1.0,0.0,0.0,1.0 }, it.second.second.c_str());
					if (ImGui::BeginPopupContextWindow(0, 1, false)) {
						if (ImGui::MenuItem("Copy"))
							Utils::ShortCutDialogs::Copy(it.second.second);
						ImGui::EndPopup();
					}
					ImGui::EndChildFrame();
					if (pos % 2 == 0)
						ImGui::PopStyleColor();

					ImGui::PopID();

				}
				else if (it.second.first == 1) {// warn
					ImGui::PushID(pos);
					if (pos % 2 == 0)
						ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.15f,0.15f,0.15f,1.0f });
					ImGui::BeginChildFrame(pos + 1, { ImGui::GetWindowWidth(),27 });
					ImGui::TextColored({ 1.0,0.635,0.0,1.0 }, it.second.second.c_str());
					if (ImGui::BeginPopupContextWindow(0, 1, false)) {
						if (ImGui::MenuItem("Copy"))
							Utils::ShortCutDialogs::Copy(it.second.second);
						ImGui::EndPopup();
					}
					ImGui::EndChildFrame();
					if (pos % 2 == 0)
						ImGui::PopStyleColor();

					ImGui::PopID();
				}
				else if (it.second.first == 2) {// INFO
					ImGui::PushID(pos);
					if (pos % 2 == 0)
						ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.15f,0.15f,0.15f,1.0f });
					ImGui::BeginChildFrame(pos + 1, { ImGui::GetWindowWidth(),27 });
					if (ImGui::BeginPopupContextWindow(0, 1, false)) {
						if (ImGui::MenuItem("Copy"))
							Utils::ShortCutDialogs::Copy(it.second.second);
						ImGui::EndPopup();
					}
					ImGui::TextColored({ 0.0,1.0,0.0,1.0 }, it.second.second.c_str());
					ImGui::EndChildFrame();
					if (pos % 2 == 0)
						ImGui::PopStyleColor();

					ImGui::PopID();
				}
				else if (it.second.first == 3) { // trace
					ImGui::PushID(pos);
					if (pos % 2 == 0)
						ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.15f,0.15f,0.15f,1.0f });
					ImGui::BeginChildFrame(pos + 1, { ImGui::GetWindowWidth(),27 });
					if (ImGui::BeginPopupContextWindow(0, 1, false)) {
						if (ImGui::MenuItem("Copy"))
							Utils::ShortCutDialogs::Copy(it.second.second);
						ImGui::EndPopup();
					}
					ImGui::TextColored({ 1.0,1.0,1.0,1.0 }, it.second.second.c_str());
					ImGui::EndChildFrame();
					if (pos % 2 == 0)
						ImGui::PopStyleColor();

					ImGui::PopID();
				}
				else { // CRITITCAL
					ImGui::PushID(pos);

					ImGui::PushStyleColor(ImGuiCol_FrameBg, { 1,1,0,1 });
					ImGui::BeginChildFrame(pos + 1, { ImGui::GetWindowWidth(),27 });
					if (ImGui::BeginPopupContextWindow(0, 1, false)) {
						if (ImGui::MenuItem("Copy"))
							Utils::ShortCutDialogs::Copy(it.second.second);
						ImGui::EndPopup();
					}
					ImGui::TextColored({ 1,0,0,1 }, it.second.second.c_str());
					ImGui::EndChildFrame();

					ImGui::PopStyleColor();
					ImGui::PopID();
				}
				pos += 1;
			}
			ImGui::PopStyleVar();
			if (Log::NewLog == true && ImGui::IsWindowFocused() == false) {
				ImGui::SetScrollHere();
				Log::NewLog = false;
			}

		}
		ImGui::End();
		ImGui::PopStyleVar();
		if (m_ShowLogSettings == false)
			return;

		if (ImGui::Begin("Log Settings", &m_ShowLogSettings)) {
			// KEYBOARD
			{
				if (ImGui::Button("KeyBoard Log Settings")) {
					ImGui::OpenPopup("KeyBoard Log");
				}
				if (ImGui::BeginPopup("KeyBoard Log")) {
					if (ExternalAPI::ImGUIAPI::CheckBox("All Events", &m_ShowAllKeyBoardEvents.ShowAll)) {
						if (m_ShowAllKeyBoardEvents.ShowAll == false) {
							m_ShowAllKeyBoardEvents.Clicked = false;
							m_ShowAllKeyBoardEvents.Released = false;
							m_ShowAllKeyBoardEvents.Pressed = false;
							m_ShowAllKeyBoardEvents.DoubleClicked = false;
						}
						else {
							m_ShowAllKeyBoardEvents.Clicked = true;
							m_ShowAllKeyBoardEvents.Released = true;
							m_ShowAllKeyBoardEvents.Pressed = true;
							m_ShowAllKeyBoardEvents.DoubleClicked = true;
						}
					}
					ExternalAPI::ImGUIAPI::CheckBox("Clicked", &m_ShowAllKeyBoardEvents.Clicked);
					ExternalAPI::ImGUIAPI::CheckBox("Released", &m_ShowAllKeyBoardEvents.Released);
					ExternalAPI::ImGUIAPI::CheckBox("Pressed", &m_ShowAllKeyBoardEvents.Pressed);
					ExternalAPI::ImGUIAPI::CheckBox("DoubleClicked", &m_ShowAllKeyBoardEvents.DoubleClicked);
					if (m_ShowAllKeyBoardEvents.DoubleClicked == false || m_ShowAllKeyBoardEvents.Pressed == false
						|| m_ShowAllKeyBoardEvents.Released == false || m_ShowAllKeyBoardEvents.Clicked == false)
						m_ShowAllKeyBoardEvents.ShowAll = false;

					if (m_ShowAllKeyBoardEvents.DoubleClicked == true || m_ShowAllKeyBoardEvents.Pressed == true
						|| m_ShowAllKeyBoardEvents.Released == true || m_ShowAllKeyBoardEvents.Clicked == true)
						m_ShowAllKeyBoardEvents.ShowOne = true;


					ImGui::EndPopup();
				}
			}

			// Mouse
			{
				if (ImGui::Button("Mouse Log Settings")) {
					ImGui::OpenPopup("Mouse Log");
				}
				if (ImGui::BeginPopup("Mouse Log")) {
					if (ExternalAPI::ImGUIAPI::CheckBox("All Events", &m_ShowAllMouseEvents.ShowAll)) {
						if (m_ShowAllMouseEvents.ShowAll == false) {
							m_ShowAllMouseEvents.Clicked = false;
							m_ShowAllMouseEvents.Released = false;
							m_ShowAllMouseEvents.Pressed = false;
							m_ShowAllMouseEvents.DoubleClicked = false;
							m_ShowAllMouseEvents.Scroll = false;
							m_ShowAllMouseEvents.Movement = false;
						}
						else {
							m_ShowAllMouseEvents.Clicked = true;
							m_ShowAllMouseEvents.Released = true;
							m_ShowAllMouseEvents.Pressed = true;
							m_ShowAllMouseEvents.DoubleClicked = true;
							m_ShowAllMouseEvents.Scroll = true;
							m_ShowAllMouseEvents.Movement = true;
						}
					}

					ExternalAPI::ImGUIAPI::CheckBox("Clicked", &m_ShowAllMouseEvents.Clicked);
					ExternalAPI::ImGUIAPI::CheckBox("Released", &m_ShowAllMouseEvents.Released);
					ExternalAPI::ImGUIAPI::CheckBox("Pressed", &m_ShowAllMouseEvents.Pressed);
					ExternalAPI::ImGUIAPI::CheckBox("DoubleClicked", &m_ShowAllMouseEvents.DoubleClicked);
					ExternalAPI::ImGUIAPI::CheckBox("Move", &m_ShowAllMouseEvents.Movement);
					ExternalAPI::ImGUIAPI::CheckBox("Scroll", &m_ShowAllMouseEvents.Scroll);
					if (m_ShowAllMouseEvents.DoubleClicked == false || m_ShowAllMouseEvents.Pressed == false
						|| m_ShowAllMouseEvents.Released == false || m_ShowAllMouseEvents.Clicked == false ||
						m_ShowAllMouseEvents.Scroll == false || m_ShowAllMouseEvents.Movement == false)
						m_ShowAllMouseEvents.ShowAll = false;

					if (m_ShowAllMouseEvents.DoubleClicked == true || m_ShowAllMouseEvents.Pressed == true
						|| m_ShowAllMouseEvents.Released == true || m_ShowAllMouseEvents.Clicked == true ||
						m_ShowAllMouseEvents.Scroll == true || m_ShowAllMouseEvents.Movement == true)
						m_ShowAllMouseEvents.ShowOne = true;


					ImGui::EndPopup();
				}
			}

			// Window
			{
				if (ImGui::Button("Window Log Settings")) {
					ImGui::OpenPopup("Window Log");
				}
				if (ImGui::BeginPopup("Window Log")) {
					if (ExternalAPI::ImGUIAPI::CheckBox("All Events", &m_ShowAllWindowEvents.ShowAll)) {
						if (m_ShowAllWindowEvents.ShowAll == false) {
							m_ShowAllWindowEvents.Resize = false;
							m_ShowAllWindowEvents.Minimize = false;
							m_ShowAllWindowEvents.Move = false;
							m_ShowAllWindowEvents.Close = false;
							m_ShowAllWindowEvents.Focus = false;
						}
						else {
							m_ShowAllWindowEvents.Resize = true;
							m_ShowAllWindowEvents.Minimize = true;
							m_ShowAllWindowEvents.Move = true;
							m_ShowAllWindowEvents.Close = true;
							m_ShowAllWindowEvents.Focus = true;
						}
					}
					ExternalAPI::ImGUIAPI::CheckBox("Resize", &m_ShowAllWindowEvents.Resize);
					ExternalAPI::ImGUIAPI::CheckBox("Minimize", &m_ShowAllWindowEvents.Minimize);
					ExternalAPI::ImGUIAPI::CheckBox("Move", &m_ShowAllWindowEvents.Move);
					ExternalAPI::ImGUIAPI::CheckBox("Close", &m_ShowAllWindowEvents.Close);
					ExternalAPI::ImGUIAPI::CheckBox("Focus", &m_ShowAllWindowEvents.Focus);

					if (m_ShowAllWindowEvents.Resize == false || m_ShowAllWindowEvents.Minimize == false
						|| m_ShowAllWindowEvents.Move == false || m_ShowAllWindowEvents.Close == false ||
						m_ShowAllWindowEvents.Focus == false)
						m_ShowAllWindowEvents.ShowAll = false;

					if (m_ShowAllWindowEvents.Resize == true || m_ShowAllWindowEvents.Minimize == true
						|| m_ShowAllWindowEvents.Move == true || m_ShowAllWindowEvents.Close == true ||
						m_ShowAllWindowEvents.Focus == true)
						m_ShowAllWindowEvents.ShowOne = true;
					ImGui::EndPopup();
				}
			}

			// Controller
			{
				if (ImGui::Button("Controller Log Settings")) {
					ImGui::OpenPopup("Controller Log");
				}
				if (ImGui::BeginPopup("Controller Log")) {
					if (ExternalAPI::ImGUIAPI::CheckBox("All Events", &m_ShowAllControllerEvents.ShowAll)) {
						if (m_ShowAllControllerEvents.ShowAll == false) {
							m_ShowAllControllerEvents.Clicked = false;
							m_ShowAllControllerEvents.Released = false;
							m_ShowAllControllerEvents.Pressed = false;
							m_ShowAllControllerEvents.DoubleClicked = false;
							m_ShowAllControllerEvents.Joystick = false;
							m_ShowAllControllerEvents.Trigger = false;
						}
						else {
							m_ShowAllControllerEvents.Clicked = true;
							m_ShowAllControllerEvents.Released = true;
							m_ShowAllControllerEvents.Pressed = true;
							m_ShowAllControllerEvents.DoubleClicked = true;
							m_ShowAllControllerEvents.Joystick = true;
							m_ShowAllControllerEvents.Trigger = true;
						}
					}
					ExternalAPI::ImGUIAPI::CheckBox("Clicked", &m_ShowAllControllerEvents.Clicked);
					ExternalAPI::ImGUIAPI::CheckBox("Released", &m_ShowAllControllerEvents.Released);
					ExternalAPI::ImGUIAPI::CheckBox("Pressed", &m_ShowAllControllerEvents.Pressed);
					ExternalAPI::ImGUIAPI::CheckBox("DoubleClicked", &m_ShowAllControllerEvents.DoubleClicked);
					ExternalAPI::ImGUIAPI::CheckBox("Joystick", &m_ShowAllControllerEvents.Joystick);
					ExternalAPI::ImGUIAPI::CheckBox("Trigger", &m_ShowAllControllerEvents.Trigger);

					if (m_ShowAllControllerEvents.DoubleClicked == false || m_ShowAllControllerEvents.Pressed == false
						|| m_ShowAllMouseEvents.Released == false || m_ShowAllControllerEvents.Clicked == false ||
						m_ShowAllControllerEvents.Joystick == false || m_ShowAllControllerEvents.Trigger == false)
						m_ShowAllControllerEvents.ShowAll = false;

					if (m_ShowAllControllerEvents.DoubleClicked == true || m_ShowAllControllerEvents.Pressed == true
						|| m_ShowAllControllerEvents.Released == true || m_ShowAllControllerEvents.Clicked == true ||
						m_ShowAllControllerEvents.Joystick == true || m_ShowAllControllerEvents.Trigger == true)
						m_ShowAllControllerEvents.ShowOne = true;
					ImGui::EndPopup();
				}
			}
		}
		ImGui::End();
	}
	void Editore3D::ViewPort() {
		PF_PROFILE_FUNC();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		static bool Open = true;
		if (ImGui::Begin("ViewPort", &Open, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar)) {
			m_ViewPoartHoveredorFocused = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
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
			ImGui::Image((ImTextureID)Text, ImVec2{ _ViewPortSize.x,_ViewPortSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			// GUIZMOS

			Entity selectedEntity = m_WorldHierachy.GetSelectedEntity();
			if (selectedEntity.GetEntityID() != 0) {
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

						glm::vec3 deltaRotation = rotation - glm::vec3{ tc.Rotation };
						tc.Location = translation;
						tc.Rotation += {glm::degrees(deltaRotation.x), glm::degrees(deltaRotation.y), glm::degrees(deltaRotation.z)};
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

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(fmt::format("{}{}", "AssetType::", "Mesh").c_str())) {
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
			ImGui::Image((ImTextureID)Renderer3DPBR::GetRenderer()->m_DeferedRendering.GPosition->GetAssetID(),{ImGui::GetWindowSize().x,ImGui::GetWindowSize().y},ImVec2{0,1},ImVec2{1,0});
		}
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0,0});
		if (ImGui::Begin("Colour")) {
			ImGui::Image((ImTextureID)Renderer3DPBR::GetRenderer()->m_DeferedRendering.GAlbedo->GetAssetID(),{ImGui::GetWindowSize().x,ImGui::GetWindowSize().y},ImVec2{0,1},ImVec2{1,0});
		}
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,ImVec2{0,0});
		if (ImGui::Begin("Normal")) {
			ImGui::Image((ImTextureID)Renderer3DPBR::GetRenderer()->m_DeferedRendering.GNormal->GetAssetID(),{ImGui::GetWindowSize().x,ImGui::GetWindowSize().y},ImVec2{0,1},ImVec2{1,0});
		}
		ImGui::End();
		ImGui::PopStyleVar();
		*/
	}

	void Editore3D::MainToolBar() {
		ImGui::Begin("##MainToolBar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse/* | ImGuiWindowFlags_NoMove*/);
		Count<Texture2D> icon;
		if (ActiveWorld->m_CurrentState == WorldState::Edit)
			icon = m_PlayButtonTexture;
		else if (ActiveWorld->m_CurrentState == WorldState::Play)
			icon = m_StopButtonTexture;
		else
			icon = m_PlayButtonTexture;

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.4f);
		ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.2f);
		if (ImGui::ImageButton((ImTextureID)icon->GetID(), ImVec2{ ImGui::GetWindowSize().y * 0.7f,ImGui::GetWindowSize().y * 0.5f })) {
			if (ActiveWorld->m_CurrentState == WorldState::Edit)
				PlayWorld();
			else if (ActiveWorld->m_CurrentState == WorldState::Play)
				SetWorldEdit();
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.5f);
		ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.2f);

		if (ImGui::ImageButton((ImTextureID)m_PauseButtonTexture->GetID(), ImVec2{ ImGui::GetWindowSize().y * 0.7f,ImGui::GetWindowSize().y * 0.5f })) {
			if (ActiveWorld->m_CurrentState == WorldState::Play)
				PauseWorld();
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.6f);
		ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.2f);

		if (ImGui::ImageButton((ImTextureID)m_SimulateButtonTexture->GetID(), ImVec2{ ImGui::GetWindowSize().y * 0.7f,ImGui::GetWindowSize().y * 0.5f })) {
			if (ActiveWorld->m_CurrentState == WorldState::Edit)
				SimulateWorld();
		}
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
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else {
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Proof Engine", p_open, window_flags);

		if (!opt_padding)
			ImGui::PopStyleVar();
		if (opt_fullscreen)
			ImGui::PopStyleVar(2);
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 100.0f; // sets the minimum width of everything

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {

				if (ImGui::MenuItem("New Project")) { // gonna be implemented later
					// gonna be implemented later
				}
				if (ImGui::MenuItem("Open Project")) {
					// gonna be implemented later
				}
				if (ImGui::MenuItem("Save", "ctrl+s")) {
					Save();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View")) {
				ImGui::MenuItem("Heirachy", nullptr, &m_WorldHierachy.m_ShowWindow);
				ImGui::MenuItem("Log", nullptr, &m_ShowLogger);
				ImGui::MenuItem("Content Browser", nullptr, &m_ContentBrowserPanel.m_ShowWindow);
				ImGui::MenuItem("Asset Manager ", nullptr, &m_AssetManagerPanel.m_ShowWindow);
				ImGui::MenuItem("Render Stats", nullptr, &m_ShowRendererStats);
				ImGui::MenuItem("World Editor", nullptr, &m_ShowWorldEditor);
				ImGui::MenuItem("Input Panel", nullptr, &m_InputPanel.m_ShowWindow);
				ImGui::MenuItem("Performance Browser", nullptr, &m_PerformancePanel.m_ShowWindow);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Debug")) {
				if (ImGui::MenuItem("Reload C# Scripts")) {
					if (ActiveWorld->GetState() == WorldState::Edit)
						ScriptEngine::ReloadAssembly(ActiveWorld.get());
					else
						PF_ERROR("Can only reload c# assembly in Edit state");
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}


	void Editore3D::Save() {
		if (ActiveWorld == nullptr)
			return;
		if (ActiveWorld->m_CurrentState != WorldState::Edit)
			PF_ERROR("Cannot save when in runtime mode");

		SceneSerializer scerelizer(ActiveWorld.get());
		scerelizer.SerilizeText(ActiveWorld->GetPath());

		PF_ENGINE_TRACE("{} Saved", ActiveWorld->GetName().c_str());

	}
	void Editore3D::PlayWorld() {
		ActiveWorld = World::Copy(m_EditorWorld);
		SceneCoreClasses::s_CurrentWorld = ActiveWorld.get();

		ActiveWorld->m_CurrentState = WorldState::Play;
		m_WorldHierachy.SetContext(ActiveWorld.get());
		m_WorldRenderer.SetContext(ActiveWorld);

		if (m_ClearLogOnPlay)
			Log::Logs.clear();
		//GuizmoType = 0;
		m_WorldHierachy.m_SelectedEntity = {};
		//Entity  entity = ActiveWorld->CreateEntity("script");
		//entity.AddComponent<RigidBodyComponent>();
		//entity.AddComponent<ScriptComponent>()->AddScript("Game.Player");
		ActiveWorld->StartRuntime();
	}
	void Editore3D::SimulateWorld() {

		ActiveWorld->m_CurrentState = WorldState::Simulate;
	}
	void Editore3D::SetWorldEdit() {

		//GuizmoType = 0;
		ActiveWorld->EndRuntime();
		ActiveWorld = m_EditorWorld;
		m_WorldHierachy.SetContext(ActiveWorld.get());
		m_WorldRenderer.SetContext(ActiveWorld);
		SceneCoreClasses::s_CurrentWorld = ActiveWorld.get();
	}
	void Editore3D::PauseWorld() {
		ActiveWorld->m_CurrentState = WorldState::Pause;
	}

	bool Editore3D::CreateAssetEditor(Asset* asset) {
		if (asset == nullptr)return false;
		auto it = m_AllPanels.find(asset->GetAssetID());
		if (it != m_AllPanels.end()) {
			it->second->SetWindowVisibile(true);
			return false;
		}
		switch (asset->GetAssetType()) {
			case Proof::AssetType::Mesh:
				{
					SceneRendererUI* temp = new SceneRendererUI(dynamic_cast<MeshAsset*>(asset));
					m_AllPanels.insert({ asset->GetAssetID(),temp });
					return true;
				}
			case Proof::AssetType::Texture:
				{
					break;
				}
			case Proof::AssetType::Material:
				{
					MaterialEditorPanel* temp = new MaterialEditorPanel(dynamic_cast<MaterialAsset*>(asset));
					m_AllPanels.insert({ asset->GetAssetID(),temp });
					return true;
				}
			case Proof::AssetType::World:
				break;
			case Proof::AssetType::MeshSourceFile:
				break;
			case Proof::AssetType::PhysicsMaterial:
				{
					PhysicsMaterialEditorPanel* temp = new PhysicsMaterialEditorPanel(dynamic_cast<PhysicsMaterialAsset*>(asset));
					m_AllPanels.insert({ asset->GetAssetID(),temp });
					return true;
				}
			default:
				break;
		}
		return false;
	}
}

