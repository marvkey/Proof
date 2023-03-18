#include "Editor3D.h"

#include <ImGui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ImGuizmo.h"
#include "Proof/Utils/PlatformUtils.h"
#include "Proof/Renderer/Renderer.h"

#include "Proof/Math/MathResource.h"
#include "Proof/Scene/Material.h"
#include "Proof/Scene/Script.h"
#include "ImGui/imgui_impl_vulkan.h"
#include <Windows.h>
#include <stdio.h> 
#include "ImGUIAPI.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Utils/PlatformUtils.h"
#include "MainWindow/SceneRendererUI.h"
#include "MainWindow/MaterialEditorPanel.h"
#include "Proof/Scene/Material.h"
#include "Proof/Math/Random.h"
#include <string>
#include "Proof/Core/Core.h"
#include<GLFW/glfw3.h>
#include "Proof/Asset/AssetManager.h"
#include <algorithm>
#include "Proof/Input/InputManager.h"
#include<thread>
#include <chrono>
#include "Proof/Core/SceneCoreClasses.h"
#include <GLFW/glfw3.h>
#include "Proof/Input/Mouse.h"

#include "Proof/Scripting/ScriptEngine.h"
namespace Proof
{
	static bool SaveSceneDialouge = false;
	Editore3D::Editore3D() :
		Layer("Editor3D Layer") {
	}
	Editore3D::~Editore3D() {
	}
	bool Editore3D::IsKeyPressedEditor(KeyBoardKey key) {
		//if (glfwGetKey((GLFWwindow*)Application::Get()->GetWindow()->GetWindow(), (int)Key)) {
		//	return  true;
		//}
		//return false;
		return Input::IsKeyPressed(key);
	}
	bool Editore3D::IsKeyClickedEditor(KeyBoardKey key) {
		//return std::find(Application::Get()->GetWindow()->KeyboardClicked.begin(), Application::Get()->GetWindow()->KeyboardClicked.end(), Key)
		//	!=
		//	Application::Get()->GetWindow()->KeyboardClicked.end();
		return Input::IsKeyClicked(key);
	}
	
	
	void Editore3D::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		if (m_ActiveWorld->m_CurrentState == WorldState::Play)
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

		m_ActiveWorld = Count<World>::Create();
		auto startworld = Application::Get()->GetProject()->GetConfig().StartWorld;
		if (AssetManager::HasAsset(startworld)) {
			auto Info = AssetManager::GetAssetInfo(startworld);
			SceneSerializer scerelizer(m_ActiveWorld.Get());
			auto path = Application::Get()->GetProject()->GetAssetFileSystemPath(Info.Path);
			if (scerelizer.DeSerilizeText(path.string()) == true) {
				m_WorldHierachy.SetContext(m_ActiveWorld.Get());
				AssetManager::LoadMultipleAsset(scerelizer.GetAssetLoadID());
			}
		}
		//ScriptEngine::ReloadAssembly(m_ActiveWorld.Get());
		SceneSerializer scerelizer(m_ActiveWorld.Get());
		


		m_WorldHierachy.SetContext(m_ActiveWorld.Get());
		m_WorldRenderer = CreateSpecial<WorldRenderer>(m_ActiveWorld, Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight());
		// cannot be setting it to window size and stuff innit
		m_EditorWorld = m_ActiveWorld;
		SceneCoreClasses::s_CurrentWorld = m_ActiveWorld.Get();

		m_PlayButtonTexture = Texture2D::Create("Resources/Icons/MainPanel/PlayButton.png");
		m_PauseButtonTexture = Texture2D::Create("Resources/Icons/MainPanel/PauseButton .png");
		m_SimulateButtonTexture = Texture2D::Create("Resources/Icons/MainPanel/SimulateButton.png");
		m_StopButtonTexture = Texture2D::Create("Resources/Icons/MainPanel/StopButton.png");

	}
	void Editore3D::OnDetach() {
		if (m_EditorWorld != nullptr) { // using editor world in case active world is on play
			SceneSerializer scerelizer(m_EditorWorld.Get());
			auto assetInfo = AssetManager::GetAssetInfo(m_EditorWorld->GetID());
			scerelizer.SerilizeText(Application::Get()->GetProject()->GetAssetFileSystemPath(assetInfo.Path).string());
		}
	}
	Count<ScreenFrameBuffer> buffer;
	void Editore3D::OnUpdate(FrameTime DeltaTime) {
		PF_PROFILE_FUNC();
		Layer::OnUpdate(DeltaTime);
		if (m_IsViewPortResize && m_ViewPortSize.x>0 && m_ViewPortSize.y>0) {
			m_WorldRenderer->Resize({(uint32_t) m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y });
			// so the camera can be edited while beig resized
			Application::Get()->GetWindow()->SetWindowInputEvent(false);
			m_EditorCamera.OnUpdate(DeltaTime, (uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			Application::Get()->GetWindow()->SetWindowInputEvent(true);
			m_IsViewPortResize = false;
		}
		switch (m_ActiveWorld->GetState()) {
			case Proof::WorldState::Play:
				{
					m_ActiveWorld->OnUpdateRuntime(DeltaTime);
					if (m_ActiveWorld->HasWorldCamera() && Mouse::IsMouseCaptured()) {
						auto entity = m_ActiveWorld->GetWorldCameraEntity();
						auto location = m_ActiveWorld->GetWorldLocation(entity);
						auto rotation = m_ActiveWorld->GetWorldRotation(entity);
						entity.GetComponent<CameraComponent>()->Width = m_ViewPortSize.x;
						entity.GetComponent<CameraComponent>()->Height = m_ViewPortSize.y;
						entity.GetComponent<CameraComponent>()->CalculateProjection(location, rotation);
						m_WorldRenderer->Render(*entity.GetComponent<CameraComponent>(), location);
					}
					else
					{
						m_EditorCamera.OnUpdate(DeltaTime, (uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
						m_WorldRenderer->Render(m_EditorCamera);
					}
					break;
				}
			case Proof::WorldState::Pause:
				{
					//m_ActiveWorld->OnUpdateRuntime(DeltaTime);
					m_EditorCamera.OnUpdate(DeltaTime, (uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
					break;
				}
			case Proof::WorldState::Simulate:
				{
					m_ActiveWorld->OnSimulatePhysics(DeltaTime);
					if (m_ViewPortFocused)
						m_EditorCamera.OnUpdate(DeltaTime, (uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
					m_WorldRenderer->Render(m_EditorCamera);
					break;
				}
			case Proof::WorldState::Edit:
				{
					m_ActiveWorld->OnUpdateEditor(DeltaTime);
					if (m_ViewPortFocused) {
						Application::Get()->GetWindow()->SetWindowInputEvent(true);
						m_EditorCamera.OnUpdate(DeltaTime, (uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
						Application::Get()->GetWindow()->SetWindowInputEvent(false);
					}
					m_WorldRenderer->Render(m_EditorCamera);
					break;
				}
			default:
				break;
		}
	}

	void Editore3D::OnImGuiDraw(FrameTime DeltaTime) {
		PF_PROFILE_FUNC();

		Layer::OnImGuiDraw(DeltaTime);
		//ImGui::ShowDemoWindow();
		static bool EnableDocking = true;
		SetDocking(&EnableDocking);
		MainToolBar();

		ViewPort();

		for (auto& a : m_AllPanels) {
			a.second->ImGuiRender(DeltaTime);
		}
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
					//m_ActiveWorld->CreateIBlTexture(file);
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
			//ImGui::Text("Renderer Company: %s", RendererBase::GetRenderCompany().c_str());
			//ImGui::Text("Graphics Card: %s", RendererBase::GetGraphicsCard().c_str());
			//ImGui::Text("Graphics Card Verison: %s", RendererBase::GetGraphicsCardVersion().c_str());
			ImGui::Text("%.3f ms/frame ", FrameTime::GetFrameMS());
			ImGui::Text("%.3f FPS",FrameTime::GetFrameFPS());

			//ImGui::TextColored({ 1.0,0,0,1 }, "RENDERER 3D");
			//ImGui::Text("DrawCalls %i", m_WorldRenderer.RenderData.Stats.DrawCalls);
			//ImGui::Text("Number Of MeshInstances %i", m_WorldRenderer.RenderData.Stats.Instances);
			//
			//ImGui::Text("Amount Of Directional Light %i", m_WorldRenderer.RenderData.Stats.AmountDirectionalLight);
			//ImGui::Text("Amount Of Point Light%i", m_WorldRenderer.RenderData.Stats.AmountPointLight);
			//ImGui::Text("Amount Of Spot Light %i", m_WorldRenderer.RenderData.Stats.AmountSpotLight);
			//
			//if (ImGui::Button("Reload Shader")) {
			//	RendererBase::GetShaderLibrary().ReloadeShaders();
			//}

			RendererBase::GetShaderLibrary()->ForEachShader([&](Count<Shader> shader) {
				ImGui::Text(shader->GetName().c_str());
				ImGui::SameLine();
				if (ImGui::Button("Reload"))
				{
					shader->Reload();
				}
			});
		
		}
		ImGui::End();
	}

	void Editore3D::OnKeyClicked(KeyClickedEvent& e) {
		// Shortcuts

		bool control = IsKeyPressedEditor(KeyBoardKey::LeftControl) || IsKeyPressedEditor(KeyBoardKey::RightControl);
		bool shift = IsKeyPressedEditor(KeyBoardKey::LeftShift) || IsKeyPressedEditor(KeyBoardKey::RightShift);
		
		//basically means that m_editor camera is beign used 
		if (m_ViewPortFocused and Input::IsMouseButtonPressed(MouseButton::ButtonRight))
			return;
		switch (e.GetKey()) {
			case KeyBoardKey::Escape:
			{
					if (m_ActiveWorld->IsPlaying())
					{
						Mouse::CaptureMouse(false);
					}
					break;
			}
			case KeyBoardKey::P:
			{
				if(control)
					Math::ChangeBool(m_WorldHierachy.m_ShowWindow);
				break;
			}
			case KeyBoardKey::L:
			{
				if (control)
					Math::ChangeBool(m_ShowLogger);
				break;
			}
			case KeyBoardKey::B:
			{
				if (control)
					Math::ChangeBool(m_ContentBrowserPanel.m_ShowWindow);
				break;
			}
			case KeyBoardKey::R:
			{
				if (control)
					Math::ChangeBool(m_ShowRendererStats);
				// no right button pressed that means that we are using the editor camera
				if (m_ViewPortFocused && Input::IsMouseButtonPressed(MouseButton::ButtonRight) == false)
					GuizmoType = ImGuizmo::OPERATION::UNIVERSALV2;
				break;
			}
			case KeyBoardKey::S:
				{
					if (control)
						Save();

					break;
				}

			case KeyBoardKey::D:
				{
					if (control && m_WorldHierachy.m_SelectedEntity.GetEntityID() != 0)
						m_WorldHierachy.m_SelectedEntity = m_ActiveWorld->CreateEntity(m_WorldHierachy.m_SelectedEntity);
					break;

				}
			case KeyBoardKey::Delete:
			case KeyBoardKey::Backspace:
				{
					if (m_WorldHierachy.m_SelectedEntity.GetEntityID() != 0 && (m_ViewPortFocused || m_WorldHierachy.m_WindowHoveredorFocus)) {

						if (m_ActiveWorld->GetState() == WorldState::Edit) {
							/*
							//Basically makig sure that all entities that reference this entity that is deleted their data Get sets to null
							m_ActiveWorld->ForEachEnitityWith<ScriptComponent>([&](Entity& entity) {
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
							*/
						}
						m_ActiveWorld->DeleteEntity(m_WorldHierachy.m_SelectedEntity);
						m_WorldHierachy.m_SelectedEntity = {};
					}
					break;
				}
				// copy entity
			case KeyBoardKey::C:
				{
					if (control && m_WorldHierachy.m_SelectedEntity.GetEntityID() != 0 && (m_ViewPortFocused || m_WorldHierachy.m_WindowHoveredorFocus)) {
						m_CopyEntity = m_WorldHierachy.m_SelectedEntity;
					}
					break;
				}
				// paste entity 
			case KeyBoardKey::V:
				{
					if (control && m_CopyEntity.GetEntityID() != 0 && (m_ViewPortFocused || m_WorldHierachy.m_WindowHoveredorFocus)) {
						m_WorldHierachy.m_SelectedEntity = m_ActiveWorld->CreateEntity(m_CopyEntity);
					}
					break;
				}

				// ImGuizmo
			case KeyBoardKey::Q:
				{
					// no right button pressed that means that we are using the editor camera
					if (m_ViewPortFocused && Input::IsMouseButtonPressed(MouseButton::ButtonRight) == false)
						GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
						break;
				}

			case KeyBoardKey::W:
				{
					// no right button pressed that means that we are using the editor camera
					if (m_ViewPortFocused && Input::IsMouseButtonPressed(MouseButton::ButtonRight) == false)
						GuizmoType = ImGuizmo::OPERATION::ROTATE;
						break;
				}
			case KeyBoardKey::E:
				{
					// no right button pressed that means that we are using the editor camera
					if (m_ViewPortFocused && Input::IsMouseButtonPressed(MouseButton::ButtonRight) == false)
						GuizmoType = ImGuizmo::OPERATION::SCALE;
						break;
				}
			case KeyBoardKey::Tab:
				{
					if (m_ViewPortFocused == false || m_WorldHierachy.m_SelectedEntity == false)
						break;
					Entity selected = m_WorldHierachy.m_SelectedEntity;
					if (shift == true) {
						if (selected.HasChildren()) {
							m_WorldHierachy.m_SelectedEntity = { selected.GetComponent<ChildComponent>()->m_Children[0],m_ActiveWorld.Get() };
						}
					}

					else if (selected.HasOwner()) {
						int childIndex = selected.GetOwner().GetComponent<ChildComponent>()->GetChildIndex(*selected.GetComponent<ChildComponent>());
						int numChildren = selected.GetOwner().GetComponent<ChildComponent>()->GetNumChildren() - 1;
						int childIndexAdd = 0;
						childIndexAdd += childIndex;
						if (childIndex >= numChildren)
							m_WorldHierachy.m_SelectedEntity = Entity{ selected.GetOwner().GetComponent<ChildComponent>()->m_Children[0],m_ActiveWorld.Get() };
						else if (childIndex < numChildren)
							m_WorldHierachy.m_SelectedEntity = Entity{ selected.GetOwner().GetComponent<ChildComponent>()->m_Children[childIndexAdd],m_ActiveWorld.Get() };
					}
					else if (selected.HasChildren()) {
						m_WorldHierachy.m_SelectedEntity = { selected.GetComponent<ChildComponent>()->m_Children[0],m_ActiveWorld.Get() };
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
					//ImGui::SetScrollHere();
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
					if (ImGui::BeginPopupContextWindow(0, 1)) {
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
					if (ImGui::BeginPopupContextWindow(0, 1)) {
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
					if (ImGui::BeginPopupContextWindow(0, 1)) {
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
					if (ImGui::BeginPopupContextWindow(0, 1)) {
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
					if (ImGui::BeginPopupContextWindow(0, 1)) {
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
			//	ImGui::SetScrollHere();
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

			m_ViewPortFocused = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
			m_ViewPortFocused = ImGui::IsWindowFocused();
			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();
			m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x,viewportMinRegion.y + viewportOffset.y };
			m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x,viewportMaxRegion.y + viewportOffset.y };

			ImVec2 currentviewPortPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewPortSize != *((glm::vec2*)&currentviewPortPanelSize)) {
				m_ViewPortSize = { currentviewPortPanelSize.x,currentviewPortPanelSize.y };
				m_IsViewPortResize = true;
			}


			if (ImGui::IsWindowFocused()) {
				m_ViewPortFocused = true;
				Application::Get()->GetWindow()->SetWindowInputEvent(true);
			}
			else {
				m_ViewPortFocused = false;
				Application::Get()->GetWindow()->SetWindowInputEvent(false);
			}
			const void* Text = m_WorldRenderer->GetImage().SourceImage;
			ImGui::Image((ImTextureID)Text, ImVec2{ m_ViewPortSize.x,m_ViewPortSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });


			// GUIZMOS

			// cherno game engein reveiw 22 minutes 48 seconds reference
			Entity selectedEntity = m_WorldHierachy.GetSelectedEntity();
			if (selectedEntity) {

				//ImGuizmo::SetOrthographic(true);
				//ImGuizmo::SetDrawlist();
				//
				//ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
				//	ImGui::GetWindowHeight(), ImGui::GetWindowWidth());
				ImGuizmo::SetOrthographic(true);
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);


				const glm::mat4& cameraProjection = m_EditorCamera.m_Projection;
				glm::mat4 cameraView = m_EditorCamera.m_View;

				auto& selectedentityTc = *selectedEntity.GetComponent<TransformComponent>();
				glm::mat4  selectedEntitytransform = selectedentityTc.GetLocalTransform();

				bool snap = Input::IsKeyPressed(KeyBoardKey::LeftControl);
				float snapValue = 0.5f; // Snap to 0.5m for translation/scale
				// Snap to 45 degrees for rotation
				if (GuizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue,snapValue,snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)GuizmoType, ImGuizmo::LOCAL, glm::value_ptr(selectedEntitytransform),
					nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing()) {
					Entity parent = m_ActiveWorld->TryGetEntity(selectedEntity.GetOwnerUUID());

					if (parent)
					{
						glm::mat4 parentTransform = m_ActiveWorld->GetWorldTransform(parent);
						selectedEntitytransform = glm::inverse(parentTransform) * selectedEntitytransform;
						glm::vec3 translation, rotation, scale;
						MathResource::DecomposeTransform(selectedEntitytransform, translation, rotation, scale);

						glm::vec3 deltaRotation = rotation - ProofToglmVec(  selectedentityTc.Rotation );
						selectedentityTc.Location =GlmVecToProof( translation);
						selectedentityTc.Rotation += Vector{Math::Degrees(deltaRotation).x, Math::Degrees(deltaRotation).y, Math::Degrees(deltaRotation).z};
						selectedentityTc.Scale =GlmVecToProof( scale);

					}
					else
					{
						glm::vec3 translation, rotation, scale;
						MathResource::DecomposeTransform(selectedEntitytransform, translation, rotation, scale);

						glm::vec3 deltaRotation = rotation - ProofToglmVec(selectedentityTc.Rotation);
						selectedentityTc.Location =GlmVecToProof( translation);
						selectedentityTc.Rotation += Vector{ Math::Degrees(deltaRotation).x, Math::Degrees(deltaRotation).y, Math::Degrees(deltaRotation).z };
						selectedentityTc.Scale = GlmVecToProof(scale);
					}
				
				}
			}
			static bool meshSourceAdded = false;
			static std::filesystem::path meshSourcePath;	
			/* putting this underneath image because a window only accpet drop tarGet to when item is bound so and image has been bound */
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::World).c_str())) {
					UUID ID = *(UUID*)payload->Data;

					m_EditorWorld = Count<World>::Create();
					m_ActiveWorld = m_EditorWorld;
					m_WorldRenderer->SetContext(m_ActiveWorld);
					m_WorldHierachy.SetContext(m_ActiveWorld.Get());
					SceneCoreClasses::s_CurrentWorld = m_ActiveWorld.Get();
					m_WorldHierachy.m_SelectedEntity = {};

					SceneSerializer ScerilizerNewWorld(m_ActiveWorld.Get());
					ScerilizerNewWorld.DeSerilizeText(ID);

				}

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Mesh).c_str())) {
					UUID meshID = *(UUID*)payload->Data;

					Entity newentt = m_ActiveWorld->CreateEntity(AssetManager::GetAssetInfo(meshID).GetName());
					newentt.AddComponent<MeshComponent>()->SetMesh(meshID);
					m_WorldHierachy.m_SelectedEntity = newentt;
				}

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::MeshSourceFile).c_str())) {
					UUID meshSourceId = *(UUID*)payload->Data;
					meshSourceAdded = true;
					meshSourcePath = AssetManager::GetAssetInfo(meshSourceId).Path;
				}
				ImGui::EndDragDropTarget();
			}
			if (meshSourceAdded) {
				AssetID id;
				std::tie(meshSourceAdded, id) = m_ContentBrowserPanel.AddMesh(AssetManager::GetAsset<MeshSource>(meshSourcePath));
				// basically add mesh is done with its operation and no longer renderng
				if (meshSourceAdded == false) {
					Entity newentt = m_ActiveWorld->CreateEntity(AssetManager::GetAssetInfo(id).GetName());
					newentt.AddComponent<MeshComponent>()->SetMesh(id);
					m_WorldHierachy.m_SelectedEntity = newentt;
				}
			}
			if (SaveSceneDialouge)
			{
				AssetID id;
				std::tie(SaveSceneDialouge,id ) = m_ContentBrowserPanel.AddWorld(m_ActiveWorld);
				if (SaveSceneDialouge == false)
				{
					m_ActiveWorld->m_WorldID = id;
				}
			}
			/*----------------------------------------------------------------------------------------------------------------------------*/
		}
		ImGui::End();
		ImGui::PopStyleVar();

	}

	void Editore3D::MainToolBar() {
		ImGui::Begin("##MainToolBar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse/* | ImGuiWindowFlags_NoMove*/);

		Count<Texture2D> icon;
		std::string state;
		if (m_ActiveWorld->m_CurrentState == WorldState::Edit)
		{
			icon = m_PlayButtonTexture;
			state = "Play";
		}
		else if (m_ActiveWorld->m_CurrentState == WorldState::Play)
		{
			icon = m_StopButtonTexture;
			state = "Stop";
		}
		else
		{
			icon = m_PlayButtonTexture;
		}

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.4f);
		ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.2f);
		if (ImGui::Button(state.c_str(), ImVec2{ImGui::GetWindowSize().y * 0.7f,ImGui::GetWindowSize().y * 0.5f})) {
			if (m_ActiveWorld->m_CurrentState == WorldState::Edit)
				PlayWorld();
			else if (m_ActiveWorld->m_CurrentState == WorldState::Play)
				SetWorldEdit();
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.5f);
		ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.2f);

		if (ImGui::ImageButton((ImTextureID)m_PauseButtonTexture->GetImage().SourceImage, ImVec2{ ImGui::GetWindowSize().y * 0.7f,ImGui::GetWindowSize().y * 0.5f })) {
			if (m_ActiveWorld->m_CurrentState == WorldState::Play)
				PauseWorld();
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.6f);
		ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.2f);

		if (ImGui::ImageButton((ImTextureID)m_PauseButtonTexture->GetImage().SourceImage, ImVec2{ ImGui::GetWindowSize().y * 0.7f,ImGui::GetWindowSize().y * 0.5f })) {
			if (m_ActiveWorld->m_CurrentState == WorldState::Edit)
				SimulateWorld();
		}
		ImGui::End();
	}

	void Editore3D::SetDocking(bool* p_open) {
		// Note: Switch this to true to enable dockspace
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
		if (opt_fullscreen) {
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else {
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		//if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			//window_flags |= ImGuiWindowFlags_NoBackground;

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
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {

			}
			if (ImGui::BeginMenu("File")) {

				if (ImGui::MenuItem("New Project")) { // gonna be implemented later
					// gonna be implemented later
				}
				if (ImGui::MenuItem("Open Project")) {
					// gonna be implemented later
				}
				if (ImGui::MenuItem("New Scene"))
				{
					Save();
					NewWorld();
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
					if (m_ActiveWorld->GetState() == WorldState::Edit)
						ScriptEngine::ReloadAssembly(m_ActiveWorld.Get());
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
		if (m_ActiveWorld == nullptr)
			return;
		if (m_ActiveWorld->m_CurrentState != WorldState::Edit){
			PF_ERROR("Cannot save when in runtime mode");
			return;
		}
		if (!AssetManager::HasAsset(m_ActiveWorld->GetID()))
		{
			SaveSceneDialouge = true;
			return;
		}
		float timeSave;
		{
			Timer time;
			SceneSerializer scerelizer(m_ActiveWorld.Get());
			auto assetInfo = AssetManager::GetAssetInfo(m_ActiveWorld->GetID());
			scerelizer.SerilizeText(Application::Get()->GetProject()->GetAssetFileSystemPath(assetInfo.Path).string());

			timeSave = time.TimePassedMillis();
		}
		PF_EC_TRACE("{} Saved  in {} m/s", m_ActiveWorld->GetName().c_str(), timeSave);

		{
			Timer time;
			AssetManager::SaveAssetManager();
			timeSave = time.TimePassedMillis();
		}
		PF_EC_TRACE("AssetManager Saved {} m/s", timeSave);

	}
	void Editore3D::NewWorld()
	{
		m_EditorWorld = Count<World>::Create();
		m_ActiveWorld = m_EditorWorld;
		m_WorldRenderer->SetContext(m_EditorWorld);
		m_WorldHierachy.SetContext(m_ActiveWorld.Get());
		SceneCoreClasses::s_CurrentWorld = m_ActiveWorld.Get();
		m_WorldHierachy.m_SelectedEntity = {};
	}
	void Editore3D::PlayWorld() {
		m_ActiveWorld = World::Copy(m_EditorWorld);
		SceneCoreClasses::s_CurrentWorld = m_ActiveWorld.Get();

		m_ActiveWorld->m_CurrentState = WorldState::Play;
		m_WorldHierachy.SetContext(m_ActiveWorld.Get());
		m_WorldRenderer->SetContext(m_ActiveWorld);

		if (m_ClearLogOnPlay)
			Log::Logs.clear();
		//GuizmoType = 0;
		m_WorldHierachy.m_SelectedEntity = {};

		m_ActiveWorld->StartRuntime();
		Mouse::CaptureMouse(true);
	}
	void Editore3D::SimulateWorld() {

		m_ActiveWorld->m_CurrentState = WorldState::Simulate;
	}
	void Editore3D::SetWorldEdit() {
		//GuizmoType = 0;
		m_ActiveWorld->EndRuntime();
		m_ActiveWorld = m_EditorWorld;
		m_WorldHierachy.SetContext(m_ActiveWorld.Get());
		m_WorldRenderer->SetContext(m_ActiveWorld);
		SceneCoreClasses::s_CurrentWorld = m_ActiveWorld.Get();
	}
	void Editore3D::PauseWorld() {
		m_ActiveWorld->m_CurrentState = WorldState::Pause;
	}

	

	bool Editore3D::CreateAssetEditor(AssetID ID) {
		auto it = m_AllPanels.find(ID);
		if (it != m_AllPanels.end()) {
			it->second->SetWindowVisibile(true);
			return false;
		}
		auto assetInfo = AssetManager::GetAssetInfo(ID);
		switch (assetInfo.Type) {
			case Proof::AssetType::Mesh:
			case Proof::AssetType::MeshSourceFile:
				{
					//Count<SceneRendererUI> panel= Count<SceneRendererUI>::Create(ID);
					//m_AllPanels.insert({ ID,panel});
					return true;
				}
			case Proof::AssetType::Texture:
				{
					break;
				}
			case Proof::AssetType::Material:
				{

					Count<Panel> panel= Count<MaterialEditorPanel>::Create(AssetManager::GetAsset<Material>(ID));
					m_AllPanels.insert({ ID,panel });
					return true;
				}
			case Proof::AssetType::World:
				break;
			case Proof::AssetType::PhysicsMaterial:
				{
					Count<Panel> panel = Count<PhysicsMaterialEditorPanel>::Create (AssetManager::GetAsset<PhysicsMaterial>(ID));
					m_AllPanels.insert({ ID,panel });
					return true;
				}
			default:
				break;
		}
		return false;
	}
}
