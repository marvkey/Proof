#include "Editor3D.h"

#include <ImGui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Proof/ProofCore.h"
#include "Proof/Scene/SceneSerializer.h"
#include "ImGuizmo.h"
#include "Proof/Utils/PlatformUtils.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Math/Vector.h"
#include "Proof/Math/MathResource.h"
#include "EditorResources.h"
#include "Proof/Scene/Script.h"
#include <thread>
#include <future>
#include <algorithm>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <stdio.h> 
#include "Proof/Utils/PlatformUtils.h"

#include "Proof/Renderer/Renderer.h"
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
#include "Proof/Renderer/Font.h"
#include "Proof/Scene/Prefab.h"

#include "Proof/Project/ProjectSerilizer.h"
#include "Proof/Renderer/ParticleSystem.h"
#include "Proof/Renderer/Renderer2D.h"

#include "Proof/Core/Profile.h"


#include "Proof/Core/Application.h"
#include "Proof/Project/Project.h"
#include "Proof/Input/Input.h"

#include "Editors/Panels/PanelManager.h"
#include "Editors/Panels/AssetManagerPanel.h"
#include "Editors/Panels/InputPanel.h"
#include "Editors/Panels/SceneHierachyPanel.h"
#include "Editors/Panels/ContentBrowserPanel.h"

#include "Editors/AssetEditors/AssetEditor.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Core/Timer.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Proof/ImGui/UI.h"
#include "Proof/Scripting/ScriptBuilder.h"

#define SCENE_HIERARCHY_PANEL_ID "SceneHierarchyPanel"
#define ECS_DEBUG_PANEL_ID "ECSDebugPanel"
#define CONSOLE_PANEL_ID "EditorConsolePanel"
#define CONTENT_BROWSER_PANEL_ID "ContentBrowserPanel"
#define INPUT_PANEL_ID "InputPanel"
#define ASSET_MANAGER_PANEL_ID "AssetManagerPanel"
#define SCENE_RENDERER_PANEL_ID "SceneRendererPanel"
namespace Proof
{
	// you can do this
	enum class ConsoleMessageFlags : int16_t
	{
		None = -1,
		Info = BIT(0),
		Warning = BIT(1),
		Error = BIT(2),

		All = Info | Warning | Error
	};
	struct EditorData
	{

		bool ShowLogSettings = false;
		bool ClearLogOnPlay = false;
		bool Docking = false;
		bool ShowLogger = true;
		bool ShowRendererStats = true;
		bool ShowWorldEditor = false;

		int GuizmoType = (1u << 0) | (1u << 1) | (1u << 2);// imguizmo bit stuff
		//SceneHierachyPanel WorldHierachy;
		//ContentBrowserPanel ContentBrowserPanel;
		//AssetManagerPanel AssetManagerPanel;
		//PerformancePanel PerformancePanel = { false };
		//InputPanel InputPanel;

		Special<PanelManager> PanelManager = CreateSpecial<class PanelManager>();
		Count<Texture2D> PlayButtonTexture;
		Count<Texture2D> PauseButtonTexture;
		Count<Texture2D> SimulateButtonTexture;
		Count<Texture2D> StopButtonTexture;
		RenderSettings RenderSettings;


	};
	enum class PopupState
	{
		None = 0,
		Rendering,
		Ok,
		Cancel
	};

	static PopupState PopModel(const std::string& ID, const std::string& label, std::string& editableText, const std::string& hint = "", const std::string& nonEditable = "",
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_CharsNoBlank, ImGuiViewport* viewport = ImGui::GetMainViewport())
	{
		UI::ScopedID scopedId(ID.c_str());
		PopupState State = PopupState::Rendering;
		ImVec2 center = viewport->GetCenter();

		if (ImGui::IsPopupOpen(ID.c_str()) == false)
			ImGui::OpenPopup(ID.c_str());
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal(ID.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{

			if (!nonEditable.empty())
			{
				ImGui::Text(nonEditable.c_str());
				ImGui::SameLine();
			}

			ImGui::Text(label.c_str());
			ImGui::SameLine();
			char buffer[1024];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), editableText.c_str());
			if (!hint.empty())
			{
				if (ImGui::InputTextWithHint("##Hash", hint.c_str(), buffer, sizeof(buffer)), flags, ImGuiInputTextFlags_CallbackAlways)
				{
					editableText = buffer;
				}
			}
			else
			{
				if (ImGui::InputText("##Hash", buffer, sizeof(buffer)), flags)
				{
					editableText = buffer;
				}
			}
			ImGui::SetItemDefaultFocus();
			ImGui::Separator();
			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				State = PopupState::Ok;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				State = PopupState::Cancel;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		return State;
	}
	static EditorData* s_EditorData = nullptr;
	static bool s_DetachPlayer = false;
	static bool SaveSceneDialouge = false;
	Editore3D* Editore3D::s_Instance = nullptr;
	PopupState NewProjectState = PopupState::None;
	std::filesystem::path NewProjectDir;
	std::string NewProjectName;
	Editore3D::Editore3D() :
		Layer("Editor3D Layer") {
		s_Instance = this;
		s_EditorData = new EditorData();
	//	s_EditorData->ContentBrowserPanel.m_ShowWindow = false;
	}
	Editore3D::~Editore3D() {
		delete s_EditorData;
		s_EditorData = nullptr;
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

		dispatcher.Dispatch<ControllerConnectEvent>([](auto& e) {
			PF_INFO(e.ToString());
			return false;
		});
		dispatcher.Dispatch<ControllerDisconnectEvent>([](auto& e) {
			PF_INFO(e.ToString());
			return false;
		});


		// KEYBOARD
		{
			if (m_ShowAllKeyBoardEvents.ShowOne == true && e.IsInCategory(EventCategory::EventKeyBoard))
			{
				if (m_ShowAllKeyBoardEvents.ShowAll == true)
				{
					PF_INFO(e.ToString());
				}
				if (m_ShowAllKeyBoardEvents.Clicked)
				{
					dispatcher.Dispatch<KeyClickedEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;

					});
				}

				if (m_ShowAllKeyBoardEvents.Released)
				{
					dispatcher.Dispatch<KeyReleasedEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;

					});
				}

				if (m_ShowAllKeyBoardEvents.DoubleClicked)
				{
					dispatcher.Dispatch<KeyDoubleClickEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}

				if (m_ShowAllKeyBoardEvents.Pressed)
				{
					dispatcher.Dispatch<KeyPressedEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}
				return;
			}
		}

		// MOUSE
		{
			if (m_ShowAllMouseEvents.ShowOne == true && e.IsInCategory(EventCategory::EventMouse))
			{
				if (m_ShowAllMouseEvents.ShowAll == true)
				{
					PF_INFO(e.ToString());
					return;
				}
				if (m_ShowAllMouseEvents.Movement)
				{
					dispatcher.Dispatch<MouseMoveEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}
				if (m_ShowAllMouseEvents.Clicked)
				{
					dispatcher.Dispatch<MouseButtonClickedEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}

				if (m_ShowAllMouseEvents.Released)
				{
					dispatcher.Dispatch<MouseButtonReleasedEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}

				if (m_ShowAllMouseEvents.DoubleClicked)
				{
					dispatcher.Dispatch<MouseButtonDoubleClickEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}

				if (m_ShowAllMouseEvents.Pressed)
				{
					dispatcher.Dispatch<MouseButtonPressedEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}

				if (m_ShowAllMouseEvents.Scroll)
				{
					dispatcher.Dispatch<MouseScrollEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}
				return;
			}
		}

		// WINDOW
		{
			if (m_ShowAllWindowEvents.ShowOne == true && e.IsInCategory(EventCategory::EventCategoryWindow))
			{
				if (m_ShowAllWindowEvents.ShowAll == true)
				{
					PF_INFO(e.ToString());
					return;
				}
				if (m_ShowAllWindowEvents.Resize)
				{
					dispatcher.Dispatch<WindowResizeEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}
				if (m_ShowAllWindowEvents.Minimize)
				{
					dispatcher.Dispatch<WindowMinimizeEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}

				if (m_ShowAllWindowEvents.Move)
				{
					dispatcher.Dispatch<WindowMoveEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}

				if (m_ShowAllWindowEvents.Close)
				{
					dispatcher.Dispatch<WindowCloseEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}

				if (m_ShowAllWindowEvents.Focus)
				{
					dispatcher.Dispatch<WindowFocusEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}
				return;
			}
		}

		// Controller
		{
			if (m_ShowAllControllerEvents.ShowOne == true && e.IsInCategory(EventCategory::EventController))
			{
				if (m_ShowAllMouseEvents.ShowAll == true)
				{
					PF_INFO(e.ToString());
					return;
				}
				if (m_ShowAllControllerEvents.Clicked)
				{
					dispatcher.Dispatch<ControllerButtonClickedEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}
				if (m_ShowAllControllerEvents.Released)
				{
					dispatcher.Dispatch<ControllerButtonReleasedEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}

				if (m_ShowAllControllerEvents.Pressed)
				{
					dispatcher.Dispatch<ControllerButtonPressedEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}

				if (m_ShowAllControllerEvents.DoubleClicked)
				{
					dispatcher.Dispatch<ControllerButtonDoubleClickEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}

				if (m_ShowAllControllerEvents.Joystick)
				{
					dispatcher.Dispatch<ControllerLeftJoystickAxisEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
					dispatcher.Dispatch<ControllerRightJoystickAxisEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}

				if (m_ShowAllControllerEvents.Trigger)
				{
					dispatcher.Dispatch<ControllerTriggerAxisEvent>([](auto& e) {
						PF_INFO(e.ToString());
						return false;
					});
				}
				return;
			}
		}



		s_EditorData->PanelManager->OnEvent(e);
		AssetEditorPanel::OnEvent(e);

		if (m_ActiveWorld->m_CurrentState == WorldState::Play)
			InputManager::OnEvent(e);

		dispatcher.Dispatch<KeyClickedEvent>(PF_BIND_FN(Editore3D::OnKeyClicked));

	}
	void Editore3D::OnAttach() {
		EditorResources::Init();
		m_ActiveWorld = Count<World>::Create();
		auto startworld = Application::Get()->GetProject()->GetConfig().StartWorldEdit;
		if (AssetManager::HasAsset(startworld))
		{
			auto Info = AssetManager::GetAssetInfo(startworld);
			SceneSerializer scerelizer(m_ActiveWorld.Get());
			auto path = Application::Get()->GetProject()->GetAssetFileSystemPath(Info.Path);
			if (scerelizer.DeSerilizeText(path.string()) == true)
			{
				AssetManager::LoadMultipleAsset(scerelizer.GetAssetLoadID());
			}
		}
		//ScriptEngine::ReloadAssembly(m_ActiveWorld.Get());
		SceneSerializer scerelizer(m_ActiveWorld.Get());
		m_EditorWorld = m_ActiveWorld;

		s_EditorData->PanelManager->AddPanel< SceneHierachyPanel>(SCENE_HIERARCHY_PANEL_ID, "Scene Hierarchy", true);
		s_EditorData->PanelManager->AddPanel<ContentBrowserPanel>(CONTENT_BROWSER_PANEL_ID, "Content Browser", true);
		s_EditorData->PanelManager->AddPanel<AssetManagerPanel>(ASSET_MANAGER_PANEL_ID, "Asset Manager", false);
		s_EditorData->PanelManager->AddPanel<InputPanel>(INPUT_PANEL_ID, "Input Panel", false);
		s_EditorData->PanelManager->SetWorldContext(m_EditorWorld);

		AssetEditorPanel::RegisterDefaultEditors();
		m_WorldRenderer = Count<WorldRenderer>::Create();

		// cannot be setting it to window size and stuff innit
		SceneCoreClasses::s_CurrentWorld = m_ActiveWorld.Get();

		s_EditorData->PlayButtonTexture = Texture2D::Create(TextureConfiguration(), "Resources/Icons/MainPanel/PlayButton.png");
		s_EditorData->PauseButtonTexture = Texture2D::Create(TextureConfiguration(), "Resources/Icons/MainPanel/PauseButton .png");
		s_EditorData->SimulateButtonTexture = Texture2D::Create(TextureConfiguration(), "Resources/Icons/MainPanel/SimulateButton.png");
		s_EditorData->StopButtonTexture = Texture2D::Create(TextureConfiguration(), "Resources/Icons/MainPanel/StopButton.png");

		m_PlayersCount = 1;

		m_ViewPortSize = { 100,100 };

	}
	void Editore3D::OnDetach() {
		if (m_EditorWorld != nullptr) // using editor world in case active world is on play 
		{
			SceneSerializer scerelizer(m_EditorWorld.Get());
			auto assetInfo = AssetManager::GetAssetInfo(m_EditorWorld->GetID());
			scerelizer.SerilizeText(Application::Get()->GetProject()->GetAssetFileSystemPath(assetInfo.Path).string());
		}
		AssetEditorPanel::UnregisterAllEditors();
		AssetManager::SaveAllAssets();
		EditorResources::Unizilize();
	}

	void Editore3D::OnUpdate(FrameTime DeltaTime) {
		PF_PROFILE_FUNC();
		Layer::OnUpdate(DeltaTime);
		m_WorldRenderer->SetViewportSize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		if (m_IsViewPortResize && m_ViewPortSize.x > 0 && m_ViewPortSize.y > 0)
		{
			m_IsViewPortResize = false;
		}
		AssetEditorPanel::OnUpdate(DeltaTime);

		switch (m_ActiveWorld->GetState())
		{
			case Proof::WorldState::Play:
				{
					m_ActiveWorld->OnUpdateRuntime(DeltaTime);
					if (m_ViewPortFocused)
					{
						Application::Get()->GetWindow()->SetWindowInputEvent(true);
						m_EditorCamera.OnUpdate(DeltaTime, (uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
						Application::Get()->GetWindow()->SetWindowInputEvent(false);
					}
					m_ActiveWorld->OnRenderEditor(m_WorldRenderer, DeltaTime, m_EditorCamera);
					#if 0
					int player = 1;
					if (m_PlayersCount > 1 && s_DetachPlayer == false && m_ActiveWorld->GetNumComponents<PlayerInputComponent>() > 0)
					{

						m_ActiveWorld->ForEachEnitityWith<PlayerInputComponent>([&](Entity entity) {
							PlayerInputComponent& input = entity.GetComponent<PlayerInputComponent>();
							if ((int)input.InputPlayer < m_PlayersCount && input.InputPlayer == Players::Player0)
							{
								Entity cameraEntity = entity.GetCamera();
								if (cameraEntity)
								{
									auto& camera = cameraEntity.GetComponent<CameraComponent>();
									uint32_t windowWIdth = m_ViewPortSize.x;
									uint32_t windowHeight = m_ViewPortSize.y;
									auto location = m_ActiveWorld->GetWorldLocation(cameraEntity);
									Vector rotation;
									if (camera.UseLocalRotation)
										rotation = cameraEntity.GetComponent<TransformComponent>().Rotation;
									else
										rotation = m_ActiveWorld->GetWorldRotation(cameraEntity);
									camera.Width = windowWIdth;
									camera.Height = windowHeight;
									camera.CalculateProjection(location, rotation);
									m_WorldRenderer->Clear();
									std::vector<std::future<void>> renders;
									if (m_MultiplayerRender.contains(input.InputPlayer))
									{
										if (m_IsViewPortResize)
											m_MultiplayerRender[input.InputPlayer]->Resize({ windowWIdth, windowHeight });
										if (entity.HasComponent<PlayerHUDComponent>())
										{
											//renders.push_back(std::async(std::launch::async, [&]() {
											m_MultiplayerRender[input.InputPlayer]->Render(camera, location, s_EditorData->RenderSettings, entity.GetComponent<PlayerHUDComponent>().HudTable);
										//}));
										}
										else
										{
											//renders.push_back(std::async(std::launch::async, [&]() {
											m_MultiplayerRender[input.InputPlayer]->Render(camera, location, s_EditorData->RenderSettings);
										//}));

										}
										//if(renders)
									}
								}
							}
						});
					}
					else if (m_ActiveWorld->HasWorldCamera() && s_DetachPlayer == false)
					{
						auto entity = m_ActiveWorld->GetWorldCameraEntity();
						auto location = m_ActiveWorld->GetWorldLocation(entity);
						Vector rotation;
						if (entity.GetComponent<CameraComponent>().UseLocalRotation)
							rotation = entity.GetComponent<TransformComponent>().Rotation;
						else
							rotation = m_ActiveWorld->GetWorldRotation(entity);
						entity.GetComponent<CameraComponent>().Width = m_ViewPortSize.x;
						entity.GetComponent<CameraComponent>().Height = m_ViewPortSize.y;
						entity.GetComponent<CameraComponent>().CalculateProjection(location, rotation);
						if (!entity.HasComponent<PlayerHUDComponent>())
							m_WorldRenderer->Render(entity.GetComponent<CameraComponent>(), location, s_EditorData->RenderSettings);
						else
							m_WorldRenderer->Render(entity.GetComponent<CameraComponent>(), location, s_EditorData->RenderSettings, entity.GetComponent<PlayerHUDComponent>().HudTable);
					}
					else
					{
						m_EditorCamera.OnUpdate(DeltaTime, (uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
						m_WorldRenderer->Render(m_EditorCamera, s_EditorData->RenderSettings);
					}
					m_ActiveWorld->OnUpdateRuntime(DeltaTime);
					#endif

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
					#if 0
					m_ActiveWorld->OnSimulatePhysics(DeltaTime);
					if (m_ViewPortFocused)
						m_EditorCamera.OnUpdate(DeltaTime, (uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
					m_WorldRenderer->Render(m_EditorCamera, s_EditorData->RenderSettings);
					#endif
					break;
				}
			case Proof::WorldState::Edit:
				{
					m_ActiveWorld->OnUpdateEditor(DeltaTime);
					if (m_ViewPortFocused)
					{
						Application::Get()->GetWindow()->SetWindowInputEvent(true);
						m_EditorCamera.OnUpdate(DeltaTime, (uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
						Application::Get()->GetWindow()->SetWindowInputEvent(false);
					}
					m_ActiveWorld->OnRenderEditor(m_WorldRenderer, DeltaTime, m_EditorCamera);
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
		Logger();


		ViewPort();
		s_EditorData->PanelManager->OnImGuiRender();
		AssetEditorPanel::OnImGuiRender();
		// handlepop
		{

			if (NewProjectState == PopupState::Rendering)
			{
				NewProjectState = PopModel("New Project", "", NewProjectName, "Name", NewProjectDir.string() + "/" + NewProjectName);
				if (NewProjectState == PopupState::Ok)
				{
					if (!NewProjectName.empty())
					{

						ProjectConfig config(NewProjectName, NewProjectDir);
						Count<Project> newProject = Project::New(config);
					}
				}
				if (NewProjectState != PopupState::Rendering)
				{
					NewProjectName = "";
					NewProjectDir = "";
					NewProjectState = PopupState::None;
				}
			}
		}

		{
			//ImGui::Begin("Render Speed Panel");
			//ImGui::Text("SetMeshPass %f", m_WorldRenderer->GetRenderer3DPBR()->GetPorformanceProfiler().SetMeshPass);
			//ImGui::Text("RenderMesh %f", m_WorldRenderer->GetRenderer3DPBR()->GetPorformanceProfiler().RenderMesh);
			//ImGui::Text("RenderMeshMaterial %f", m_WorldRenderer->GetRenderer3DPBR()->GetPorformanceProfiler().RenderMeshMaterial);
			//ImGui::End();
		}

		a:
		if (s_EditorData->ShowRendererStats == false)
			return;
		ImGui::Begin("Renderer Stastitics", &s_EditorData->ShowRendererStats);
		{
			PF_PROFILE_FUNC("Renderer Stastitics");

			UI::BeginPropertyGrid("Rendere Stats");
			{
				UI::AttributeText("Shadow Settings");
			}

			UI::AttributeBool("View COlliders", s_EditorData->RenderSettings.ViewColliders);

			UI::AttributeTextBar("##Frame",fmt::format("{} ms/frame", FrameTime::GetFrameMS()));
			UI::AttributeTextBar("##FPS", fmt::format("{} FPS", FrameTime::GetFrameFPS()));


			Renderer::GetShaderLibrary()->ForEachShader([&](Count<Shader> shader) {
				UI::ScopedID id(shader->GetName().c_str());

				if(UI::AttributeButton(shader->GetName(),"Reload"))
					shader->Reload();
			});
			{
				UI::EnumCombo("Collider View", m_WorldRenderer->Options.ShowPhysicsColliders);
			}
			//ExternalAPI::ImGUIAPI::EnumCombo<WorldRendererOptions::PhysicsColliderView>("Physics Collider", m_WorldRenderer->Options.ShowPhysicsColliders);
			UI::EndPropertyGrid();

			UI::BeginPropertyGrid("Shadow settign grid");

			
			UI::AttributeText("Shadow Settings");

			UI::AttributeBool("DebugPass", m_WorldRenderer->ShadowSetting.RenderDebugPass);
			if (m_WorldRenderer->ShadowSetting.RenderDebugPass)
			{

				UI::Image(m_WorldRenderer->GetShadowPassDebugImage(),
					{ ImGui::GetWindowWidth(),ImGui::GetContentRegionAvail().y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
				UI::AttributeSlider("Cascade Index", m_WorldRenderer->ShadowSetting.DebugCascade, 0, 3);
			}

			ShadowSetting& shadowSetting = m_WorldRenderer->ShadowSetting;
			UI::AttributeBool("ShowCascades", shadowSetting.ShowCascades);
			UI::AttributeBool("Soft Shadows", shadowSetting.SoftShadows);
			UI::AttributeDrag("Max Shadow Distance", shadowSetting.MaxShadowDistance);
			UI::AttributeDrag("Shadow Fade", shadowSetting.ShadowFade, 5.0f);

			// cascade settings tre node
			UI::AttributeBool("CascadeFading", shadowSetting.CascadeFading);
			if (shadowSetting.CascadeFading)
				UI::AttributeDrag("CascadeTransitionFade", shadowSetting.CascadeTransitionFade, 0.05);

			UI::AttributeDrag("CascadeSplitLambda", shadowSetting.CascadeSplitLambda, 0.01);
			UI::AttributeDrag("CascadeNearPlaneOffset", shadowSetting.CascadeNearPlaneOffset, 0.1, Math::GetMinType<float>(), 0, "%.3f", ImGuiSliderFlags_AlwaysClamp);
			UI::AttributeDrag("CascadeFarPlaneOffset", shadowSetting.CascadeFarPlaneOffset, 0.1, 0, Math::GetMaxType<float>(), "%.3f", ImGuiSliderFlags_AlwaysClamp);
			UI::AttributeDrag("ScaleShadowCascadesToOrigin", shadowSetting.ScaleShadowCascadesToOrigin, 0.1, 0, Math::GetMaxType<float>(), "%.3f", ImGuiSliderFlags_AlwaysClamp);

			ImGui::Checkbox("UseManualCascadeSplits", &shadowSetting.UseManualCascadeSplits);

			if (shadowSetting.UseManualCascadeSplits)
			{
				UI::AttributeDrag("Cascade 0", shadowSetting.CascadeSplits[0], 0.025);
				UI::AttributeDrag("Cascade 1", shadowSetting.CascadeSplits[1], 0.025);
				UI::AttributeDrag("Cascade 2", shadowSetting.CascadeSplits[2], 0.025);
				UI::AttributeDrag("Cascade 3", shadowSetting.CascadeSplits[3], 0.025);
			}
			UI::EndPropertyGrid();

		}

		ImGui::End();
	}

	bool Editore3D::OnKeyClicked(KeyClickedEvent& e) {
		// Shortcuts

		bool control = IsKeyPressedEditor(KeyBoardKey::LeftControl) || IsKeyPressedEditor(KeyBoardKey::RightControl);
		bool shift = IsKeyPressedEditor(KeyBoardKey::LeftShift) || IsKeyPressedEditor(KeyBoardKey::RightShift);

		//basically means that m_editor camera is beign used 
		if (m_ViewPortFocused == false || Input::IsMouseButtonPressed(MouseButton::ButtonRight) == true)
			return false;
		switch (e.GetKey())
		{
			case KeyBoardKey::Escape:
				{
					if (m_ActiveWorld->IsPlaying())
					{
						Mouse::CaptureMouse(false);
						s_DetachPlayer = true;
						return true;
						break;
					}
					break;
				}
			case KeyBoardKey::F:
				{
					m_EditorCamera.SetPosition(	s_EditorData->PanelManager->GetPanel<SceneHierachyPanel>(SCENE_HIERARCHY_PANEL_ID)->GetSelectedEntity().Transform().Location);
					return true;
				}
			case KeyBoardKey::P:
				{
					if (control)
					{
						Math::ChangeBool(s_EditorData->PanelManager->GetPanelData(SCENE_HIERARCHY_PANEL_ID)->IsOpen);
						return true;
					}
					break;
				}
			case KeyBoardKey::L:
				{
					if (control)
					{
						Math::ChangeBool(s_EditorData->ShowLogger);
						return true;
					}
					break;
				}
			case KeyBoardKey::B:
				{
					if (control)
					{
						Math::ChangeBool(s_EditorData->PanelManager->GetPanelData(CONTENT_BROWSER_PANEL_ID)->IsOpen);
						return true;
					}
					break;
				}
			case KeyBoardKey::R:
				{
					if (control)
					{

						Math::ChangeBool(s_EditorData->ShowRendererStats);
						return true;
					}
				// no right button pressed that means that we are using the editor camera
					if (m_ViewPortFocused && Input::IsMouseButtonPressed(MouseButton::ButtonRight) == false)
						s_EditorData->GuizmoType = ImGuizmo::OPERATION::UNIVERSALV2;
					break;
				}
			case KeyBoardKey::S:
				{
					if (control)
					{
						Save();
						return true;
					}

					break;
				}

			case KeyBoardKey::D:
				{
					//if (control && s_EditorData->WorldHierachy.m_SelectedEntity.GetUUID() != 0)
					//	s_EditorData->WorldHierachy.m_SelectedEntity = m_ActiveWorld->CreateEntity(s_EditorData->WorldHierachy.m_SelectedEntity);
					break;

				}
			case KeyBoardKey::Delete:
			case KeyBoardKey::Backspace:
				{
					/*
					if (s_EditorData->WorldHierachy.m_SelectedEntity.GetUUID() != 0 && (m_ViewPortFocused || s_EditorData->WorldHierachy.m_WindowHoveredorFocus))
					{

						if (m_ActiveWorld->GetState() == WorldState::Edit) {
							/*
							//Basically makig sure that all entities that reference this entity that is deleted their data Get sets to null
							m_ActiveWorld->ForEachEnitityWith<ScriptComponent>([&](Entity& entity) {
								auto& scp = entity.GetComponent<ScriptComponent>();
								for (auto& scripts : scp.m_Scripts) {
									for (auto& field : scripts.Fields) {
										if (field.Type == ProofMonoType::Enum) {
											uint64_t* data = field.Data._Cast<uint64_t>();
											if (data == nullptr)
												return;
											if (*data == s_EditorData->WorldHierachy.m_SelectedEntity.GetUUID())
												*data = 0;
										}
									}
								}
							});
						}
						m_ActiveWorld->DeleteEntity(s_EditorData->WorldHierachy.m_SelectedEntity);
						s_EditorData->WorldHierachy.m_SelectedEntity = {};
					}
					*/
					break;
				}
				// copy entity
			case KeyBoardKey::C:
				{
					//if (control && s_EditorData->WorldHierachy.m_SelectedEntity.GetUUID() != 0 && (m_ViewPortFocused || s_EditorData->WorldHierachy.m_WindowHoveredorFocus)) {
					//	m_CopyEntity = s_EditorData->WorldHierachy.m_SelectedEntity;
					//}
					break;
				}
				// paste entity 
			case KeyBoardKey::V:
				{
					//if (control && m_CopyEntity.GetUUID() != 0 && (m_ViewPortFocused || s_EditorData->WorldHierachy.m_WindowHoveredorFocus)) {
					//	s_EditorData->WorldHierachy.m_SelectedEntity = m_ActiveWorld->CreateEntity(m_CopyEntity);
					//}
					break;
				}

				// ImGuizmo
			case KeyBoardKey::Q:
				{
					// no right button pressed that means that we are using the editor camera
					if (m_ViewPortFocused && Input::IsMouseButtonPressed(MouseButton::ButtonRight) == false)
						s_EditorData->GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
					break;
				}

			case KeyBoardKey::W:
				{
					// no right button pressed that means that we are using the editor camera
					if (m_ViewPortFocused && Input::IsMouseButtonPressed(MouseButton::ButtonRight) == false)
					{
						s_EditorData->GuizmoType = ImGuizmo::OPERATION::ROTATE;
						return true;
					}
					break;
				}
			case KeyBoardKey::E:
				{
					// no right button pressed that means that we are using the editor camera
					if (m_ViewPortFocused && Input::IsMouseButtonPressed(MouseButton::ButtonRight) == false)
					{
						s_EditorData->GuizmoType = ImGuizmo::OPERATION::SCALE;
						return true;
					}
					break;
				}

		}
		return false;
	}
	void Editore3D::Logger() {
		if (s_EditorData->ShowLogger == false)
			return;
		PF_PROFILE_FUNC();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		if (ImGui::Begin("Log", &s_EditorData->ShowLogger, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar))
		{
			ImGui::BeginMenuBar();
			{
				UI::AttributeBool("pause logging", Log::m_PauseLog);
				UI::AttributeBool("Clear On Play", s_EditorData->ClearLogOnPlay);
				ImGui::SameLine();
				if (ImGui::Button("Clear log"))
				{
					Log::Logs.clear();
					ImGui::SetScrollHereY();
				}
				if (ImGui::Button("Settings"))
				{

					Math::ChangeBool(s_EditorData->ShowLogSettings);
				}
			}
			ImGui::EndMenuBar();
			int pos = 0;
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 1);
			for (auto& it : Log::Logs)
			{
				if (it.second.first == 0)
				{// ERROR
					ImGui::PushID(pos);
					if (pos % 2 == 0)
						ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.15f,0.15f,0.15f,1.0f });
					ImGui::BeginChildFrame(pos + 1, { ImGui::GetWindowWidth(),27 });
					ImGui::TextColored({ 1.0,0.0,0.0,1.0 }, it.second.second.c_str());
					if (ImGui::BeginPopupContextWindow(0, 1))
					{
						if (ImGui::MenuItem("Copy"))
							ShortCutDialogs::Copy(it.second.second);
						ImGui::EndPopup();
					}
					ImGui::EndChildFrame();
					if (pos % 2 == 0)
						ImGui::PopStyleColor();

					ImGui::PopID();

				}
				else if (it.second.first == 1)
				{// warn
					ImGui::PushID(pos);
					if (pos % 2 == 0)
						ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.15f,0.15f,0.15f,1.0f });
					ImGui::BeginChildFrame(pos + 1, { ImGui::GetWindowWidth(),27 });
					ImGui::TextColored({ 1.0,0.635,0.0,1.0 }, it.second.second.c_str());
					if (ImGui::BeginPopupContextWindow(0, 1))
					{
						if (ImGui::MenuItem("Copy"))
							ShortCutDialogs::Copy(it.second.second);
						ImGui::EndPopup();
					}
					ImGui::EndChildFrame();
					if (pos % 2 == 0)
						ImGui::PopStyleColor();

					ImGui::PopID();
				}
				else if (it.second.first == 2)
				{// INFO
					ImGui::PushID(pos);
					if (pos % 2 == 0)
						ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.15f,0.15f,0.15f,1.0f });
					ImGui::BeginChildFrame(pos + 1, { ImGui::GetWindowWidth(),27 });
					if (ImGui::BeginPopupContextWindow(0, 1))
					{
						if (ImGui::MenuItem("Copy"))
							ShortCutDialogs::Copy(it.second.second);
						ImGui::EndPopup();
					}
					ImGui::TextColored({ 0.0,1.0,0.0,1.0 }, it.second.second.c_str());
					ImGui::EndChildFrame();
					if (pos % 2 == 0)
						ImGui::PopStyleColor();

					ImGui::PopID();
				}
				else if (it.second.first == 3)
				{ // trace
					ImGui::PushID(pos);
					if (pos % 2 == 0)
						ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.15f,0.15f,0.15f,1.0f });
					ImGui::BeginChildFrame(pos + 1, { ImGui::GetWindowWidth(),27 });
					if (ImGui::BeginPopupContextWindow(0, 1))
					{
						if (ImGui::MenuItem("Copy"))
							ShortCutDialogs::Copy(it.second.second);
						ImGui::EndPopup();
					}
					ImGui::TextColored({ 1.0,1.0,1.0,1.0 }, it.second.second.c_str());
					ImGui::EndChildFrame();
					if (pos % 2 == 0)
						ImGui::PopStyleColor();

					ImGui::PopID();
				}
				else
				{ // CRITITCAL
					ImGui::PushID(pos);

					ImGui::PushStyleColor(ImGuiCol_FrameBg, { 1,1,0,1 });
					ImGui::BeginChildFrame(pos + 1, { ImGui::GetWindowWidth(),27 });
					if (ImGui::BeginPopupContextWindow(0, 1))
					{
						if (ImGui::MenuItem("Copy"))
							ShortCutDialogs::Copy(it.second.second);
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
			if (Log::NewLog == true && ImGui::IsWindowFocused() == false)
			{
				ImGui::SetScrollHereY();
				Log::NewLog = false;
			}

		}
		ImGui::End();
		ImGui::PopStyleVar();
		if (s_EditorData->ShowLogSettings == false)
			return;

		if (ImGui::Begin("Log Settings", &s_EditorData->ShowLogSettings))
		{
// KEYBOARD
			{
				if (ImGui::Button("KeyBoard Log Settings"))
				{
					ImGui::OpenPopup("KeyBoard Log");
				}
				if (ImGui::BeginPopup("KeyBoard Log"))
				{
					if (UI::AttributeBool("All Events", m_ShowAllKeyBoardEvents.ShowAll))
					{
						if (m_ShowAllKeyBoardEvents.ShowAll == false)
						{
							m_ShowAllKeyBoardEvents.Clicked = false;
							m_ShowAllKeyBoardEvents.Released = false;
							m_ShowAllKeyBoardEvents.Pressed = false;
							m_ShowAllKeyBoardEvents.DoubleClicked = false;
						}
						else
						{
							m_ShowAllKeyBoardEvents.Clicked = true;
							m_ShowAllKeyBoardEvents.Released = true;
							m_ShowAllKeyBoardEvents.Pressed = true;
							m_ShowAllKeyBoardEvents.DoubleClicked = true;
						}
					}
					UI::AttributeBool("Clicked", m_ShowAllKeyBoardEvents.Clicked);
					UI::AttributeBool("Released", m_ShowAllKeyBoardEvents.Released);
					UI::AttributeBool("Pressed", m_ShowAllKeyBoardEvents.Pressed);
					UI::AttributeBool("DoubleClicked", m_ShowAllKeyBoardEvents.DoubleClicked);
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
				if (ImGui::Button("Mouse Log Settings"))
				{
					ImGui::OpenPopup("Mouse Log");
				}
				if (ImGui::BeginPopup("Mouse Log"))
				{
					if (UI::AttributeBool("All Events", m_ShowAllMouseEvents.ShowAll))
					{
						if (m_ShowAllMouseEvents.ShowAll == false)
						{
							m_ShowAllMouseEvents.Clicked = false;
							m_ShowAllMouseEvents.Released = false;
							m_ShowAllMouseEvents.Pressed = false;
							m_ShowAllMouseEvents.DoubleClicked = false;
							m_ShowAllMouseEvents.Scroll = false;
							m_ShowAllMouseEvents.Movement = false;
						}
						else
						{
							m_ShowAllMouseEvents.Clicked = true;
							m_ShowAllMouseEvents.Released = true;
							m_ShowAllMouseEvents.Pressed = true;
							m_ShowAllMouseEvents.DoubleClicked = true;
							m_ShowAllMouseEvents.Scroll = true;
							m_ShowAllMouseEvents.Movement = true;
						}
					}

					UI::AttributeBool("Clicked", m_ShowAllMouseEvents.Clicked);
					UI::AttributeBool("Released", m_ShowAllMouseEvents.Released);
					UI::AttributeBool("Pressed", m_ShowAllMouseEvents.Pressed);
					UI::AttributeBool("DoubleClicked", m_ShowAllMouseEvents.DoubleClicked);
					UI::AttributeBool("Move", m_ShowAllMouseEvents.Movement);
					UI::AttributeBool("Scroll", m_ShowAllMouseEvents.Scroll);
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
				if (ImGui::Button("Window Log Settings"))
				{
					ImGui::OpenPopup("Window Log");
				}
				if (ImGui::BeginPopup("Window Log"))
				{
					if (UI::AttributeBool("All Events", m_ShowAllWindowEvents.ShowAll))
					{
						if (m_ShowAllWindowEvents.ShowAll == false)
						{
							m_ShowAllWindowEvents.Resize = false;
							m_ShowAllWindowEvents.Minimize = false;
							m_ShowAllWindowEvents.Move = false;
							m_ShowAllWindowEvents.Close = false;
							m_ShowAllWindowEvents.Focus = false;
						}
						else
						{
							m_ShowAllWindowEvents.Resize = true;
							m_ShowAllWindowEvents.Minimize = true;
							m_ShowAllWindowEvents.Move = true;
							m_ShowAllWindowEvents.Close = true;
							m_ShowAllWindowEvents.Focus = true;
						}
					}
					UI::AttributeBool("Resize", m_ShowAllWindowEvents.Resize);
					UI::AttributeBool("Minimize", m_ShowAllWindowEvents.Minimize);
					UI::AttributeBool("Move", m_ShowAllWindowEvents.Move);
					UI::AttributeBool("Close", m_ShowAllWindowEvents.Close);
					UI::AttributeBool("Focus", m_ShowAllWindowEvents.Focus);

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
				if (ImGui::Button("Controller Log Settings"))
				{
					ImGui::OpenPopup("Controller Log");
				}
				if (ImGui::BeginPopup("Controller Log"))
				{
					if (UI::AttributeBool("All Events", m_ShowAllControllerEvents.ShowAll))
					{
						if (m_ShowAllControllerEvents.ShowAll == false)
						{
							m_ShowAllControllerEvents.Clicked = false;
							m_ShowAllControllerEvents.Released = false;
							m_ShowAllControllerEvents.Pressed = false;
							m_ShowAllControllerEvents.DoubleClicked = false;
							m_ShowAllControllerEvents.Joystick = false;
							m_ShowAllControllerEvents.Trigger = false;
						}
						else
						{
							m_ShowAllControllerEvents.Clicked = true;
							m_ShowAllControllerEvents.Released = true;
							m_ShowAllControllerEvents.Pressed = true;
							m_ShowAllControllerEvents.DoubleClicked = true;
							m_ShowAllControllerEvents.Joystick = true;
							m_ShowAllControllerEvents.Trigger = true;
						}
					}
					UI::AttributeBool("Clicked", m_ShowAllControllerEvents.Clicked);
					UI::AttributeBool("Released", m_ShowAllControllerEvents.Released);
					UI::AttributeBool("Pressed", m_ShowAllControllerEvents.Pressed);
					UI::AttributeBool("DoubleClicked", m_ShowAllControllerEvents.DoubleClicked);
					UI::AttributeBool("Joystick", m_ShowAllControllerEvents.Joystick);
					UI::AttributeBool("Trigger", m_ShowAllControllerEvents.Trigger);

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
		if (ImGui::Begin("ViewPort", &Open, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
		{

			m_ViewPortFocused = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();
			m_ViewPortFocused = ImGui::IsWindowFocused();
			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();
			m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x,viewportMinRegion.y + viewportOffset.y };
			m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x,viewportMaxRegion.y + viewportOffset.y };

			ImVec2 currentviewPortPanelSize = ImGui::GetContentRegionAvail();
			if (m_ViewPortSize != *((glm::vec2*)&currentviewPortPanelSize))
			{
				m_ViewPortSize = { currentviewPortPanelSize.x,currentviewPortPanelSize.y };
				m_IsViewPortResize = true;
			}


			if (ImGui::IsWindowFocused())
			{
				m_ViewPortFocused = true;
				Application::Get()->GetWindow()->SetWindowInputEvent(true);
			}
			else
			{
				m_ViewPortFocused = false;
				Application::Get()->GetWindow()->SetWindowInputEvent(false);
			}
			#if 0
			if (m_ActiveWorld->IsPlaying() && m_PlayersCount > 1 && s_DetachPlayer == false && m_ActiveWorld->GetNumComponents<PlayerInputComponent>() > 0)
			{
				//(input, entity ID)
				std::map<int, uint64_t> inputs;
				m_ActiveWorld->ForEachEnitityWith<PlayerInputComponent>([&](Entity entity) {
					PlayerInputComponent& input = entity.GetComponent<PlayerInputComponent>();
					if (!m_MultiplayerRender.contains(input.InputPlayer))return;
					inputs[(int)input.InputPlayer] = entity.GetUUID();
				});
				switch (m_PlayersCount)
				{
					case 4:
						{
							auto element = inputs.begin();
							UI::Image(m_MultiplayerRender[(Players)inputs.begin()->first]->GetImage(), ImVec2{ m_ViewPortSize.x,m_ViewPortSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
							//element++;
							//ImGui::SameLine();
							//ImGui::Image((ImTextureID)m_MultiplayerRender[(Players)element->first]->GetImage().SourceImage, ImVec2{ m_ViewPortSize.x/2,m_ViewPortSize.y/2 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
							//
							//element++;
							//ImGui::Image((ImTextureID)m_MultiplayerRender[(Players)element->first]->GetImage().SourceImage, ImVec2{ m_ViewPortSize.x / 2,m_ViewPortSize.y / 2 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
							//ImGui::SameLine();
							//ImGui::Image((ImTextureID)m_MultiplayerRender[(Players)element->first]->GetImage().SourceImage, ImVec2{ m_ViewPortSize.x / 2,m_ViewPortSize.y / 2 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

						}

					case 2:
						{
							auto element = inputs.begin();
							UI::Image(m_MultiplayerRender[(Players)element->first]->GetImage(), ImVec2{ m_ViewPortSize.x / 2,m_ViewPortSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

							ImGui::SameLine();
							element++;
							UI::Image(m_MultiplayerRender[(Players)element->first]->GetImage(), ImVec2{ m_ViewPortSize.x / 2,m_ViewPortSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
						}
				}
			}
			else
			{

			}
			#endif
			auto swapchain = Renderer::GetCurrentFrame().ImageIndex;
			///UI::Image(m_WorldRenderer->m_ExternalCompositeFrameBuffer->GetColorAttachmentImage(swapchain, 0), ImVec2{ m_ViewPortSize.x,m_ViewPortSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			UI::Image(m_WorldRenderer->GetFinalPassImage(), ImVec2{ m_ViewPortSize.x,m_ViewPortSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			//UI::Image(m_WorldRenderer->GetRenderer2D()->GetTargetFrameBuffer()->GetColorAttachmentImage(swapchain,0), ImVec2{ m_ViewPortSize.x,m_ViewPortSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			//UI::Image(m_WorldRenderer->m_GeometryPass->GetTargetFrameBuffer()->GetColorAttachmentImage(swapchain,0), ImVec2{ m_ViewPortSize.x,m_ViewPortSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

			//UI::Image(Renderer::GetWhiteTexture(), ImVec2{m_ViewPortSize.x,m_ViewPortSize.y}, ImVec2{0,1}, ImVec2{1,0});
			// GUIZMOS

			// cherno game engein reveiw 22 minutes 48 seconds reference
			Entity selectedEntity = s_EditorData->PanelManager->GetPanel<SceneHierachyPanel>(SCENE_HIERARCHY_PANEL_ID)->GetSelectedEntity();
			if (selectedEntity)
			{

//ImGuizmo::SetOrthographic(true);
//ImGuizmo::SetDrawlist();
//
//ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
//	ImGui::GetWindowHeight(), ImGui::GetWindowWidth());
				ImGuizmo::SetOrthographic(true);
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);


				const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();
				glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

				auto& selectedentityTc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 selectedEntitytransform = selectedentityTc.GetTransform();

				bool snap = Input::IsKeyPressed(KeyBoardKey::LeftControl);
				float snapValue = 0.5f; // Snap to 0.5m for translation/scale
				// Snap to 45 degrees for rotation
				if (s_EditorData->GuizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue,snapValue,snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)s_EditorData->GuizmoType, ImGuizmo::LOCAL, glm::value_ptr(selectedEntitytransform),
					nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing())
				{
					Entity parent = m_ActiveWorld->TryGetEntityWithUUID(selectedEntity.GetParentUUID());

					if (parent)
					{
						glm::mat4 parentTransform = m_ActiveWorld->GetWorldSpaceTransform(parent);
						selectedEntitytransform = glm::inverse(parentTransform) * selectedEntitytransform;
						glm::vec3 translation, rotation, scale;
						MathResource::DecomposeTransform(selectedEntitytransform, translation, rotation, scale);

						glm::vec3 deltaRotation = rotation - selectedentityTc.GetRotationEuler();
						selectedentityTc.Location = translation;
						selectedentityTc.SetRotation(selectedentityTc.GetRotationEuler() += deltaRotation);
						selectedentityTc.Scale = scale;

					}
					else
					{
						glm::vec3 translation, rotation, scale;
						MathResource::DecomposeTransform(selectedEntitytransform, translation, rotation, scale);

						glm::vec3 deltaRotation = rotation - selectedentityTc.GetRotationEuler();
						selectedentityTc.Location = translation;
						selectedentityTc.SetRotation(selectedentityTc.GetRotationEuler() += deltaRotation);
						selectedentityTc.Scale = scale;
					}

				}
			}
			static bool meshSourceAdded = false;
			static std::filesystem::path meshSourcePath;
			/* putting this underneath image because a window only accpet drop tarGet to when item is bound so and image has been bound */
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::World).c_str()))
				{
					UUID ID = *(UUID*)payload->Data;

					m_EditorWorld = Count<World>::Create();
					m_ActiveWorld = m_EditorWorld;
					s_EditorData->PanelManager->SetWorldContext(m_EditorWorld);
					SceneCoreClasses::s_CurrentWorld = m_ActiveWorld.Get();

					SceneSerializer ScerilizerNewWorld(m_ActiveWorld.Get());
					ScerilizerNewWorld.DeSerilizeText(ID);

				}

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Mesh).c_str()))
				{
					UUID meshID = *(UUID*)payload->Data;

					Entity newentt = m_ActiveWorld->CreateEntity(AssetManager::GetAssetInfo(meshID).GetName());
					newentt.AddComponent<MeshComponent>().SetMesh(meshID);
					s_EditorData->PanelManager->GetPanel<SceneHierachyPanel>(SCENE_HIERARCHY_PANEL_ID)->SetSelectedEntity(newentt);
				}
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::DynamicMesh).c_str()))
				{
					UUID meshID = *(UUID*)payload->Data;

					Entity newentt = m_ActiveWorld->CreateEntity(AssetManager::GetAsset<DynamicMesh>(meshID));
					s_EditorData->PanelManager->GetPanel<SceneHierachyPanel>(SCENE_HIERARCHY_PANEL_ID)->SetSelectedEntity(newentt);
				}

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::MeshSourceFile).c_str()))
				{
					UUID meshSourceId = *(UUID*)payload->Data;
					meshSourceAdded = true;
					meshSourcePath = AssetManager::GetAssetFileSystemPath(AssetManager::GetAssetInfo(meshSourceId).Path);
				}

				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EnumReflection::EnumString(AssetType::Prefab).c_str()))
				{
					UUID prefabId = *(UUID*)payload->Data;

					Count<Prefab> prefab = AssetManager::GetAsset<Prefab>(prefabId);
					std::string name = AssetManager::GetAssetInfo(prefabId).GetName();

					Entity newentt = m_ActiveWorld->CreateEntity(name, prefab, TransformComponent());
					s_EditorData->PanelManager->GetPanel<SceneHierachyPanel>(SCENE_HIERARCHY_PANEL_ID)->SetSelectedEntity(newentt);
				}
				ImGui::EndDragDropTarget();
			}

			if (meshSourceAdded)
			{
				AssetID id;
				std::tie(meshSourceAdded, id) = s_EditorData->PanelManager->GetPanel<ContentBrowserPanel>(CONTENT_BROWSER_PANEL_ID)->AddMesh(AssetManager::GetAsset<MeshSource>(meshSourcePath));
				// basically add mesh is done with its operation and no longer renderng
				if (meshSourceAdded == false)
				{
					if (AssetManager::GetAssetInfo(id).Type == AssetType::Mesh)
					{

						Entity newentt = m_ActiveWorld->CreateEntity(AssetManager::GetAssetInfo(id).GetName());
						newentt.AddComponent<MeshComponent>().SetMesh(id);
						s_EditorData->PanelManager->GetPanel<SceneHierachyPanel>(SCENE_HIERARCHY_PANEL_ID)->SetSelectedEntity(newentt);
					}
					else if (AssetManager::GetAssetInfo(id).Type == AssetType::DynamicMesh)
					{

						Entity newentt = m_ActiveWorld->CreateEntity(AssetManager::GetAssetInfo(id).GetName());
						newentt.AddComponent<DynamicMeshComponent>().SetMesh(id);
						s_EditorData->PanelManager->GetPanel<SceneHierachyPanel>(SCENE_HIERARCHY_PANEL_ID)->SetSelectedEntity(newentt);
					}
				}
			}
			if (SaveSceneDialouge)
			{
				AssetID id;
				std::tie(SaveSceneDialouge, id) = s_EditorData->PanelManager->GetPanel<ContentBrowserPanel>(CONTENT_BROWSER_PANEL_ID)->AddWorld(m_ActiveWorld);
				if (SaveSceneDialouge == false)
				{
					Save();
				}
			}
			/*----------------------------------------------------------------------------------------------------------------------------*/
		}
		ImGui::End();
		ImGui::PopStyleVar();

	}

	void Editore3D::MainToolBar() {
		PF_PROFILE_FUNC();

		ImGui::Begin("##MainToolBar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse  /* | ImGuiWindowFlags_NoMove*/);

		Count<Texture2D> icon;
		std::string state;
		if (m_ActiveWorld->m_CurrentState == WorldState::Edit)
		{
			icon = s_EditorData->PlayButtonTexture;
			state = "Play";
		}
		else if (m_ActiveWorld->m_CurrentState == WorldState::Play)
		{
			icon = s_EditorData->StopButtonTexture;
			state = "Stop";
		}
		else
		{
			icon = s_EditorData->PlayButtonTexture;
		}

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.4f);
		ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.2f);
		if (ImGui::Button(state.c_str(), ImVec2{ ImGui::GetWindowSize().y * 0.7f,ImGui::GetWindowSize().y * 0.5f }))
		{
			if (m_ActiveWorld->m_CurrentState == WorldState::Edit)
				PlayWorld();
			else if (m_ActiveWorld->m_CurrentState == WorldState::Play)
				SetWorldEdit();
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.5f);
		ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.2f);

		if (UI::ImageButton(s_EditorData->PlayButtonTexture->GetImage(), ImVec2{ ImGui::GetWindowSize().y * 0.7f,ImGui::GetWindowSize().y * 0.5f }))
		{
			if (m_ActiveWorld->m_CurrentState == WorldState::Play)
				PauseWorld();
		}
		//ImGui::SameLine();
		//ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.6f);
		//ImGui::SetCursorPosY(ImGui::GetWindowSize().y * 0.2f);
		//
		//if (UI::ImageButton(s_EditorData->PauseButtonTexture->GetImage(), ImVec2{ ImGui::GetWindowSize().y * 0.7f,ImGui::GetWindowSize().y * 0.5f })) {
		//	if (m_ActiveWorld->m_CurrentState == WorldState::Edit)
		//		SimulateWorld();
		//}
		//ImGui::SameLine();
		//int playerCount = m_PlayersCount;
		//if (ImGui::DragInt("PlayerCount", &playerCount, 1, 1, 4))
		//{
		//	m_PlayersCount = playerCount;
		//}
		// scene is playing
		if (state == "Stop")
		{
			ImGui::SameLine();
			ImGui::Checkbox("Detach Player", &s_DetachPlayer);
		}
		ImGui::End();

	}

	void Editore3D::SetDocking(bool* p_open) {
		// code taken form walnut https://github.com/TheCherno/Walnut/blob/master/Walnut/src/Walnut/Application.cpp
		PF_PROFILE_FUNC();


		Count<Project> activeProject = Application::Get()->GetProject();
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_MenuBar;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Proof Engine", nullptr, window_flags);
		ImGui::PopStyleVar();

		ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{

			}
			if (ImGui::BeginMenu("File"))
			{

				if (ImGui::MenuItem("New Project"))
				{ // gonna be implemented later
					std::filesystem::path director = FileSystem::OpenFolderDialog();
					if (!director.empty())
					{
						NewProjectName = "NewProject";
						NewProjectDir = director;
						NewProjectState = PopupState::Rendering;
					}
				}
				if (ImGui::MenuItem("Open Project"))
				{
					std::filesystem::path file = FileSystem::OpenFileDialog("Proof Project (*.ProofProject)\0*.ProofProject\0");
					if (!file.empty())
					{
						Application::Get()->OpenProject(file);
					}
				}
				if (ImGui::MenuItem("New Scene"))
				{
					Save();
					NewWorld();
				}
				if (ImGui::MenuItem("Save", "ctrl+s"))
				{
					Save();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::BeginMenu("ScriptEngine"))
				{
					if (ImGui::MenuItem("Open Visual Studio Solution", nullptr, nullptr, FileSystem::Exists(activeProject->GetScriptProjectSolutionPath())))
						FileSystem::OpenExternally(activeProject->GetScriptProjectSolutionPath());

					if (ImGui::MenuItem("Regenerate Visual Studio Solution"))
						ScriptBuilder::RegenerateProjectScriptSolution(activeProject);

					if (ImGui::MenuItem("Build C# Assembly", nullptr, nullptr, FileSystem::Exists(activeProject->GetScriptProjectSolutionPath())))
						ScriptBuilder::BuildCSProject(activeProject);

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Reload C# Assembly"))
				{
					ScriptEngine::ReloadppAssembly();
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				for (auto& [id, panelData] : s_EditorData->PanelManager->GetPanels())
				{
					if (ImGui::MenuItem(panelData.Name, nullptr, &panelData.IsOpen))
					{
						//m_PanelManager->Serialize();
					}
				}
				ImGui::MenuItem("Log", nullptr, &s_EditorData->ShowLogger);
				ImGui::MenuItem("Render Stats", nullptr, &s_EditorData->ShowRendererStats);
				ImGui::MenuItem("World Editor", nullptr, &s_EditorData->ShowWorldEditor);
				ImGui::EndMenu();
			}


			ImGui::EndMenuBar();
		}
		ImGui::End();
	}



	void Editore3D::Save() {
		if (m_ActiveWorld == nullptr)
			return;
		if (m_ActiveWorld->m_CurrentState != WorldState::Edit)
		{
			PF_ERROR("Cannot save when in runtime mode");
			return;
		}
		if (!AssetManager::HasAsset(m_ActiveWorld->GetID()))
		{
			SaveSceneDialouge = true;
			return;
		}
		{
			ScopeTimer scopeTime(fmt::format("{} Saved", m_ActiveWorld->GetName()));
			SceneSerializer scerelizer(m_ActiveWorld.Get());
			auto assetInfo = AssetManager::GetAssetInfo(m_ActiveWorld->GetID());
			scerelizer.SerilizeText(Application::Get()->GetProject()->GetAssetFileSystemPath(assetInfo.Path).string());

		}

		{
			ScopeTimer scopeTime(fmt::format("AssetManager Saved"));

			AssetManager::SaveAssetManager();
		}

		ProjectSerilizer serilizer(Application::Get()->GetProject().Get());
		if (Application::Get()->GetProject()->GetConfig().OnCloseStartWorldEditLastOpen)
			Application::Get()->GetProject()->m_ProjectConfig.StartWorldEdit = m_ActiveWorld->GetID();
		serilizer.SerilizeText(Application::Get()->GetProject()->GetConfig().Project.string());

	}
	void Editore3D::NewWorld()
	{
		m_EditorWorld = Count<World>::Create();
		m_ActiveWorld = m_EditorWorld;
		s_EditorData->PanelManager->SetWorldContext(m_ActiveWorld);
		SceneCoreClasses::s_CurrentWorld = m_ActiveWorld.Get();
	}
	void Editore3D::SetActiveWorld(Count<World> world)
	{
	}
	void Editore3D::PlayWorld() {
		m_ActiveWorld = World::Copy(m_EditorWorld);
		s_DetachPlayer = false;
		s_EditorData->PanelManager->SetWorldContext(m_ActiveWorld);
		SceneCoreClasses::s_CurrentWorld = m_ActiveWorld.Get();

		m_ActiveWorld->m_CurrentState = WorldState::Play;
		if (s_EditorData->ClearLogOnPlay)
			Log::Logs.clear();

		m_ActiveWorld->StartRuntime();

		//{
		//	
		//	for (int i = 0; i < m_PlayersCount; i++)
		//	{
		//		if (m_MultiplayerRender[(Players)(i + 1)] == nullptr)
		//			m_MultiplayerRender[(Players)(i + 1)] = Count<WorldRenderer>::Create(m_ActiveWorld, m_ViewPortSize.x, m_ViewPortSize.y);
		//		else
		//			m_MultiplayerRender[(Players)(i + 1)]->SetContext(m_ActiveWorld);
		//	}
		//}
		//Mouse::CaptureMouse(true);
	}
	void Editore3D::SimulateWorld() {
		s_DetachPlayer = false;
		m_ActiveWorld->m_CurrentState = WorldState::Simulate;
	}
	void Editore3D::SetWorldEdit() {

		//s_EditorData->GuizmoType = 0;
		m_ActiveWorld->EndRuntime();
		m_ActiveWorld = m_EditorWorld;
		s_EditorData->PanelManager->SetWorldContext(m_ActiveWorld);
		SceneCoreClasses::s_CurrentWorld = m_ActiveWorld.Get();
		s_DetachPlayer = false;
	}
	void Editore3D::PauseWorld() {
		m_ActiveWorld->m_CurrentState = WorldState::Pause;
	}
}
