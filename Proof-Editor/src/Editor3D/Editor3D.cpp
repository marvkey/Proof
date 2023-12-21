#include "Editor3D.h"
#include "Proof/ProofCore.h"
#include "Proof/Core/Profile.h"
#include "Proof/Core/Application.h"
#include "Proof/Core/Timer.h"

#include "Proof/Input/Input.h"
#include "Proof/Input/InputManager.h"
#include "Proof/Input/Mouse.h"
#include "Proof/Asset/AssetManager.h"

#include "Proof/Project/Project.h"
#include "Proof/Project/ProjectSerilizer.h"
#include "Proof/Scripting/ScriptEngine.h"
#include "Proof/Scripting/ScriptBuilder.h"
#include "Proof/Physics/PhysicsDebugger.h"
#include "Proof/Physics/PhysicsEngine.h"
#include "Proof/Math/Ray.h"
#include "Proof/Math/BasicCollision.h"

#include "Proof/Scene/SceneSerializer.h"
#include "Proof/Scene/Prefab.h"
#include "Proof/Scene/Mesh.h"

#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Renderer/Renderer.h"

#include "Proof/Utils/PlatformUtils.h"
#include "Proof/Utils/StringUtils.h"
#include "Proof/Physics/MeshCollider.h"
#include "Proof/Physics/PhysicsMeshCooker.h"

#include "Proof/ImGui/UI.h"
#include "Proof/ImGui/UiUtilities.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include "Proof/ImGui/Editors/EditorResources.h"
#include "Proof/ImGui/Editors/Panels/PanelManager.h"
#include "Proof/ImGui/Editors/Panels/AssetManagerPanel.h"
#include "Proof/ImGui/Editors/Panels/InputPanel.h"
#include "Proof/ImGui/Editors/Panels/SceneHierachyPanel.h"
#include "Proof/ImGui/Editors/Panels/ContentBrowser/ContentBrowserPanel.h"
#include "Proof/ImGui/Editors/Panels/WorldRendererPanel.h"
#include "Proof/ImGui/Editors/Panels/PhysicsPanelStats.h"
#include "Proof/ImGui/Editors/Panels/ProjectSettingsPanel.h"
#include "Proof/ImGui/Editors/AssetEditors/AssetEditor.h"
#include "Proof/ImGui/SelectionManager.h"
#include "Proof/ImGui/Editors/EditorWorkspace/EditorWorkspace.h"
#include "Proof/ImGui/Editors/EditorWorkspace/ViewPortEditorWorkspace.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SCENE_HIERARCHY_PANEL_ID "SceneHierarchyPanel"
#define ECS_DEBUG_PANEL_ID "ECSDebugPanel"
#define CONSOLE_PANEL_ID "EditorConsolePanel"
#define CONTENT_BROWSER_PANEL_ID "ContentBrowserPanel"
#define INPUT_PANEL_ID "InputPanel"
#define ASSET_MANAGER_PANEL_ID "AssetManagerPanel"
#define WORLD_RENDERER_PANEL_ID "WorldRendererPanel"
#define PHYSICS_DEBUG_PANEL_ID "PhysicsDebugPanel"
#define PROJECT_DEBUG_PANEL_ID "ProjectSettings"
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
		bool ShowStatisticsPanel = false;
		int GuizmoType = (1u << 0) | (1u << 1) | (1u << 2);// imguizmo bit stuff
		//SceneHierachyPanel WorldHierachy;
		//ContentBrowserPanel ContentBrowserPanel;
		//AssetManagerPanel AssetManagerPanel;
		//PerformancePanel PerformancePanel = { false };
		//InputPanel InputPanel;

		Special<PanelManager> PanelManager = CreateSpecial<class PanelManager>();
		Special<EditorWorkspaceManager> EditorWorkspaceManager = CreateSpecial<class EditorWorkspaceManager>();

		Count<Texture2D> PlayButtonTexture;
		Count<Texture2D> PauseButtonTexture;
		Count<Texture2D> SimulateButtonTexture;
		Count<Texture2D> StopButtonTexture;

		struct CreateNewMeshPopupData
		{
			Count<MeshSource> MeshToCreate;
			std::string CreateMeshFilenameBuffer = "Mesh/";
			std::string CreateSkeletonFilenameBuffer;
			std::string CreateAnimationFilenameBuffer;
			Entity TargetEntity;

			CreateNewMeshPopupData()
			{
				MeshToCreate = nullptr;
				TargetEntity = {};
			}

		} CreateNewMeshPopupData;

		struct SaveAsNewWorldPopupData
		{
			Count<World> WorldToSaveAs;
			bool SaveAssetManager = true;
			std::string CreateWorldFile = "Scene/";
		}CreateNewWorldPopupData;
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

	struct SelectionData
	{ 
		Entity Entity;
		//SubMesh* SubMesh = nullptr;
		float Distance = 0.0f;
	};
	std::pair<float, float> GetMouseViewportSpace(glm::vec2 viewportBounds[2] )
	{
		auto [mx, my] = ImGui::GetMousePos();
		mx -= viewportBounds[0].x;
		my -= viewportBounds[0].y;
		auto viewportWidth = viewportBounds[1].x - viewportBounds[0].x;
		auto viewportHeight = viewportBounds[1].y - viewportBounds[0].y;

		return { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
	}

	Ray CastRay(const EditorCamera& camera, float mx, float my)
	{
		glm::vec4 mouseClipPos = { mx, my, -1.0f, 1.0f };

		auto inverseProj = glm::inverse(camera.GetProjectionMatrix());
		auto inverseView = glm::inverse(glm::mat3(camera.GetViewMatrix()));

		glm::vec4 ray = inverseProj * mouseClipPos;
		glm::vec3 rayPos = camera.GetPosition();
		glm::vec3 rayDir = inverseView * glm::vec3(ray);

		return Ray{ rayPos, rayDir };
	}
	Editore3D::Editore3D() :
		Layer("Editor3D Layer") 
	{
		s_Instance = this;
		s_EditorData = pnew EditorData();
	//	s_EditorData->ContentBrowserPanel.m_ShowWindow = false;
	}
	Editore3D::~Editore3D() {
		pdelete s_EditorData;
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


	void Editore3D::OnEvent(Event& e) 
	{
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
			}
		}



		

		m_EditorCamera.OnEvent(e);

		s_EditorData->PanelManager->OnEvent(e);
		s_EditorData->EditorWorkspaceManager->OnEvent(e);
		AssetEditorPanel::OnEvent(e);

		if (m_ActiveWorld->m_CurrentState == WorldState::Play)
			InputManager::OnEvent(e);

		dispatcher.Dispatch<KeyClickedEvent>(PF_BIND_FN(Editore3D::OnKeyClicked));
		dispatcher.Dispatch<MouseButtonClickedEvent>(PF_BIND_FN(Editore3D::OnMouseButtonClicked));
	}
	void Editore3D::OnAttach() 
	{
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
		s_EditorData->PanelManager->AddPanel<PhysicsStatsPanel>(PHYSICS_DEBUG_PANEL_ID, "Physics Stats", false);
		s_EditorData->PanelManager->AddPanel<AssetManagerPanel>(ASSET_MANAGER_PANEL_ID, "Asset Manager", false);
		s_EditorData->PanelManager->AddPanel<InputPanel>(INPUT_PANEL_ID, "Input Panel", false);
		s_EditorData->PanelManager->AddPanel<WorldRendererPanel>(WORLD_RENDERER_PANEL_ID, "Renderer Panel", true);
		s_EditorData->PanelManager->AddPanel<ProjectSettingsPanel>(PROJECT_DEBUG_PANEL_ID, "Project Settings", false);

		Count< ContentBrowserPanel> contentBrowser = s_EditorData->PanelManager->AddPanel<ContentBrowserPanel>(CONTENT_BROWSER_PANEL_ID, "Content Browser", true);
		s_EditorData->PanelManager->SetWorldContext(m_EditorWorld);
		{
			//s_EditorData->EditorWorkspaceManager->AddWorkspace<("Viewport")
			s_EditorData->EditorWorkspaceManager->AddWorkspace<ViewPortEditorWorkspace>("Viewport", true, "Viewport", ViewPortEditorData{ false,false,std::bind(&Editore3D::UI_HandleAssetDrop, this) });
			s_EditorData->EditorWorkspaceManager->SetWorldContext(m_ActiveWorld);
		}
#if 0
		contentBrowser->RegisterItemActivateCallbackForType(AssetType::World, [this](const AssetInfo& metadata)
			{
				OpenScene(metadata);
			});
#endif
		contentBrowser->RegisterItemActivateCallbackForType(AssetType::ScriptFile, [this](const AssetInfo& metadata)
			{
				FileSystem::OpenExternally(AssetManager::GetAssetFileSystemPath(metadata.Path));
			});

		contentBrowser->RegisterAssetCreatedCallback([this](const AssetInfo& metadata)
			{
				if (metadata.Type == AssetType::ScriptFile)
					ScriptBuilder::RegenerateProjectScriptSolution(Application::Get()->GetProject());
			});

		contentBrowser->RegisterAssetDeletedCallback([this](const AssetInfo& metadata)
			{
				if (metadata.Type == AssetType::ScriptFile)
				ScriptBuilder::RegenerateProjectScriptSolution(Application::Get()->GetProject());

			});


		AssetEditorPanel::RegisterDefaultEditors();
		m_WorldRenderer = Count<WorldRenderer>::Create();


		s_EditorData->PlayButtonTexture = Texture2D::Create(TextureConfiguration(), "Resources/Icons/MainPanel/PlayButton.png");
		s_EditorData->PauseButtonTexture = Texture2D::Create(TextureConfiguration(), "Resources/Icons/MainPanel/PauseButton .png");
		s_EditorData->SimulateButtonTexture = Texture2D::Create(TextureConfiguration(), "Resources/Icons/MainPanel/SimulateButton.png");
		s_EditorData->StopButtonTexture = Texture2D::Create(TextureConfiguration(), "Resources/Icons/MainPanel/StopButton.png");
		s_EditorData->PanelManager->GetPanel< WorldRendererPanel>(WORLD_RENDERER_PANEL_ID)->SetContext(m_WorldRenderer);

	
		m_PlayersCount = 1;

		m_ViewPortSize = { 100,100 };

		FileSystem::StartWatching();
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
		FileSystem::StopWatching();
		FileSystem::ClearFileSystemChangedCallbacks();
	}

	void Editore3D::OnUpdate(FrameTime DeltaTime) 
	{
		PF_PROFILE_FUNC();
		Layer::OnUpdate(DeltaTime);
		m_WorldRenderer->SetViewportSize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		m_EditorCamera.SetViewportSize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);

		if (m_IsViewPortResize && m_ViewPortSize.x > 0 && m_ViewPortSize.y > 0)
		{
			m_IsViewPortResize = false;
		}
		AssetEditorPanel::OnUpdate(DeltaTime);
		s_EditorData->EditorWorkspaceManager->OnUpdate(DeltaTime);
		switch (m_ActiveWorld->GetState())
		{
			case Proof::WorldState::Play:
				{
					m_ActiveWorld->OnUpdateRuntime(DeltaTime);
					if (m_ViewPortFocused)
					{
						m_EditorCamera.SetActive(m_ViewPortFocused);
						m_EditorCamera.OnUpdate(DeltaTime);
					}
					//m_ActiveWorld->OnRenderEditor(m_WorldRenderer, DeltaTime, m_EditorCamera);
					break;
				}
			case Proof::WorldState::Pause:
				{
					//m_ActiveWorld->OnUpdateRuntime(DeltaTime);
					m_EditorCamera.SetActive(m_ViewPortFocused);
					m_EditorCamera.OnUpdate(DeltaTime);
					break;
				}
			case Proof::WorldState::Simulate:
				{
					break;
				}
			case Proof::WorldState::Edit:
				{
					m_ActiveWorld->OnUpdateEditor(DeltaTime);
					if (m_ViewPortFocused)
					{
						m_EditorCamera.SetActive(m_ViewPortFocused);
						m_EditorCamera.OnUpdate(DeltaTime);
					}
					//m_ActiveWorld->OnRenderEditor(m_WorldRenderer, DeltaTime, m_EditorCamera);
					break;
				}
			default:
				break;
		}
	}
	void Editore3D::OnImGuiDraw() 
	{
		PF_PROFILE_FUNC();

		Layer::OnImGuiDraw();
		//ImGui::ShowDemoWindow();
		static bool EnableDocking = true;
		SetDocking(&EnableDocking);
		MainToolBar();
		Logger();

		UI_StatisticsPanel();

		if (SaveSceneDialouge)
			UI_SaveWorldAs();
		//ViewPort();
		s_EditorData->PanelManager->OnImGuiRender();
		s_EditorData->EditorWorkspaceManager->OnImGuiRender();

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

	}

	bool Editore3D::OnKeyClicked(KeyClickedEvent& e) {
		// Shortcuts

		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureKeyboard) // ths means that an imgui text field is being used
			return false;

		bool control = IsKeyPressedEditor(KeyBoardKey::LeftControl) || IsKeyPressedEditor(KeyBoardKey::RightControl);
		bool shift = IsKeyPressedEditor(KeyBoardKey::LeftShift) || IsKeyPressedEditor(KeyBoardKey::RightShift);
		bool isViewportOrHierieachyFocused = UI::IsWindowFocused("Scene Hierarchy") || m_ViewPortFocused;

		
		//UI::is
		//basically means that m_editor camera is beign used 
		if (Input::IsMouseButtonPressed(MouseButton::ButtonRight) == true)
			return false;
		switch (e.GetKey())
		{
			case KeyBoardKey::Escape:
				{
					if (m_ActiveWorld->IsPlaying())
					{
						//Mouse::CaptureMouse(false);
						//s_DetachPlayer = true;
						return true;
						break;
					}
					break;
				}
			case KeyBoardKey::F:
				{
					//m_EditorCamera.SetPosition(	s_EditorData->PanelManager->GetPanel<SceneHierachyPanel>(SCENE_HIERARCHY_PANEL_ID)->GetSelectedEntity().Transform().Location);
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
					if (isViewportOrHierieachyFocused && control)
					{
						auto selectedEntities = SelectionManager::GetSelections(SelectionContext::Scene);
						for (const auto& entityID : selectedEntities)
						{
							Entity entity = m_ActiveWorld->GetEntity(entityID);
							Entity duplicate = m_ActiveWorld->CreateEntity(entity);
							SelectionManager::Deselect(SelectionContext::Scene, entity.GetUUID());  
							SelectionManager::Select(SelectionContext::Scene, duplicate.GetUUID());
						}
						if(!selectedEntities.empty())
							return true;
					}
					break;

				}
			case KeyBoardKey::Delete:
			case KeyBoardKey::Backspace:
				{
					if (isViewportOrHierieachyFocused )
					{
						auto selectedEntities = SelectionManager::GetSelections(SelectionContext::Scene);
						for (const auto& entityID : selectedEntities)
						{
							Entity entity = m_ActiveWorld->GetEntity(entityID);
							SelectionManager::Deselect(SelectionContext::Scene, entity.GetUUID());
							m_ActiveWorld->DeleteEntity(entity);
						}
						if (!selectedEntities.empty())
							return true;
					}
					break;
				}
				// copy entity
			case KeyBoardKey::C:
				{
					break;
				}
				// paste entity 
			case KeyBoardKey::V:
				{
					
					break;
				}

				// ImGuizmo
			case KeyBoardKey::Q:
				{
					// no right button pressed that means that we are using the editor camera
					if (m_ViewPortFocused && Input::IsMouseButtonPressed(MouseButton::ButtonRight) == false)
					{
						s_EditorData->GuizmoType = ImGuizmo::OPERATION::TRANSLATE;
						return true;
					}
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


	bool Editore3D::OnMouseButtonClicked(MouseButtonClickedEvent& e)
	{
		if (!m_EditorCamera.IsActive())
			return false;

		if (Input::IsKeyPressed(KeyBoardKey::LeftAlt) || Input::IsMouseButtonPressed(MouseButton::ButtonRight))
			return false;

		if (ImGuizmo::IsOver())
			return false;

		if (e.GetButton() != MouseButton::ButtonLeft)
			return false;

		ImGui::ClearActiveID();

		std::vector<SelectionData> selectionData;
		//https://github.com/InCloudsBelly/X2_RenderingEngine/blob/e7c349b70bd95af3ab673556cdb56cb2cc40b48e/Engine/X2/EditorLayer.cpp#L3340
		auto [mouseX, mouseY] = GetMouseViewportSpace(m_ViewportBounds);

		if (mouseX > -1.0f && mouseX < 1.0f && mouseY > -1.0f && mouseY < 1.0f)
		{
			const auto& camera = m_EditorCamera;
			auto [origin, direction] = CastRay(camera, mouseX, mouseY);

			auto meshEntities = m_ActiveWorld->GetAllEntitiesWith<DynamicMeshComponent>();
			for (auto e : meshEntities)
			{
				Entity entity = { e, m_ActiveWorld.Get() };
				auto& mc = entity.GetComponent<DynamicMeshComponent>();
				auto mesh = mc.GetMesh();
				if (!mesh)
					continue;

				auto& submeshes = mesh->GetMeshSource()->GetSubMeshes();
				float lastT = std::numeric_limits<float>::max();
				const auto& subMesh = submeshes[mc.GetSubMeshIndex()];
				glm::mat4 transform = m_ActiveWorld->GetWorldSpaceTransform(entity);
				Ray ray = 
				{
					glm::inverse(transform) * glm::vec4(origin, 1.0f),
					glm::inverse(glm::mat3(transform)) * direction
				};

				float t;
				bool intersects = BasicCollision::RayInAABB(ray, subMesh.BoundingBox, 0.3, 50.0, t);
				if (intersects)
				{

					selectionData.push_back(SelectionData{ entity,t });

					/*
					const auto& triangleCache = mesh->GetMeshSource()->GetTriangleCache(mc.SubmeshIndex);
					for (const auto& triangle : triangleCache)
					{
						if (ray.Intersects(triangle.V0.Position, triangle.V1.Position, triangle.V2.Position, t))
						{
							selectionData.push_back({ entity, &submesh, t });
							break;
						}
					}
					*/
				}
			}

			auto staticMeshEntities = m_ActiveWorld->GetAllEntitiesWith<MeshComponent>();
			for (auto e : staticMeshEntities)
			{
				Entity entity = { e, m_ActiveWorld.Get() };
				auto& smc = entity.GetComponent<MeshComponent>();
				auto staticMesh = smc.GetMesh();
				if (!staticMesh )
					continue;

				auto& submeshes = staticMesh->GetMeshSource()->GetSubMeshes();
				glm::mat4 transform = m_ActiveWorld->GetWorldSpaceTransform(entity);
				Ray ray =
				{
					glm::inverse(transform) * glm::vec4(origin, 1.0f),
					glm::inverse(glm::mat3(transform)) * direction
				};
				float t;
				bool intersects = BasicCollision::RayInAABB(ray, staticMesh->GetMeshSource()->GetBoundingBox(), 0.3, 50.0, t);

				if (intersects)
				{
					selectionData.push_back(SelectionData{ entity, t });
				}
# if 0
				float lastT = std::numeric_limits<float>::max();
				for (uint32_t i = 0; i < submeshes.size(); i++)
				{
					auto& subMesh = submeshes[i];
					glm::mat4 transform = m_ActiveWorld->GetWorldSpaceTransform(entity);
					Ray ray =
					{
						glm::inverse(transform) * glm::vec4(origin, 1.0f),
						glm::inverse(glm::mat3(transform)) * direction
					};

					float t;
					bool intersects = BasicCollision::RayInAABB(ray, subMesh.BoundingBox, 0.3, 50.0, t);
					if (intersects)
					{
						selectionData.push_back(SelectionData{ entity, (SubMesh*)&subMesh, t });

						/*
						const auto& triangleCache = staticMesh->GetMeshSource()->GetTriangleCache(i);
						for (const auto& triangle : triangleCache)
						{
							if (ray.Intersects(triangle.V0.Position, triangle.V1.Position, triangle.V2.Position, t))
							{
								selectionData.push_back({ entity, &submesh, t });
								break;
							}
						}
						*/
					}
				}
#endif
			}

			std::sort(selectionData.begin(), selectionData.end(), [](auto& a, auto& b) { return a.Distance > b.Distance; });

			bool ctrlDown = Input::IsKeyPressed(KeyBoardKey::LeftControl) || Input::IsKeyPressed(KeyBoardKey::RightControl);
			bool shiftDown = Input::IsKeyPressed(KeyBoardKey::LeftShift) || Input::IsKeyPressed(KeyBoardKey::RightShift);
			if (!ctrlDown)
				SelectionManager::DeselectAll();

			if (!selectionData.empty())
			{
				Entity entity = selectionData.front().Entity;
				if (shiftDown)
				{
					while (entity.GetParent())
					{
						entity = entity.GetParent();
					}
				}
				if (SelectionManager::IsSelected(SelectionContext::Scene, entity.GetUUID()) && ctrlDown)
					SelectionManager::Deselect(SelectionContext::Scene, entity.GetUUID());
				else
					SelectionManager::Select(SelectionContext::Scene, entity.GetUUID());
			}
		}

		return false;
	}
	void Editore3D::Logger() 
	{
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
	void Editore3D::ViewPort() 
	{
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
			UI::Image(m_WorldRenderer->GetFinalPassImage(), ImVec2{ m_ViewPortSize.x,m_ViewPortSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			// GUIZMOS

			// cherno game engein reveiw 22 minutes 48 seconds reference
			Entity selectedEntity;
			if (SelectionManager::GetSelections(SelectionContext::Scene).size() > 0)
				selectedEntity = m_ActiveWorld->GetEntity( SelectionManager::GetSelections(SelectionContext::Scene).front());
			if (selectedEntity)
			{
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
			UI_HandleAssetDrop();

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
		if (state == "Stop")
		{
			ImGui::SameLine();
			ImGui::Checkbox("Detach Player", &s_DetachPlayer);
		}
		ImGui::End();

	}

	void Editore3D::SetDocking(bool* p_open) 
	{
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
				if (ImGui::MenuItem("New World"))
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
				for (auto& [id, panelData] : s_EditorData->PanelManager->GetPanelsRef())
				{
					bool* open = &panelData.IsOpen;
					ImGui::MenuItem(panelData.Name, nullptr, open);
				}
				ImGui::MenuItem("Log", nullptr, &s_EditorData->ShowLogger);
				ImGui::MenuItem("Render Stats", nullptr, &s_EditorData->ShowRendererStats);
				ImGui::MenuItem("World Editor", nullptr, &s_EditorData->ShowWorldEditor);
				ImGui::MenuItem("Statistics Panel", nullptr, &s_EditorData->ShowStatisticsPanel);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Debug"))
			{
			
				if (PhysicsDebugger::IsDebugging())
				{
					if (ImGui::MenuItem("Stop PhysX Debugging"))
						PhysicsDebugger::StopDebugging();
				}
				else
				{

					if (ImGui::MenuItem("Start PhysX Debugging"))
					{
						PhysicsDebugger::StartDebugging((Application::Get()->GetProject()->GetProjectDirectory() / "PhysXDebugInfo").string(), PhysicsEngine::GetSettings().DebugType == PhysicsDebugType::Live);
					}
					UI::EnumCombo("PhysicsDebugType", PhysicsEngine::GetSettings().DebugType);
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		ImGui::End();
	}



	bool Editore3D::Save() 
	{
		if (m_ActiveWorld == nullptr)
			return false;
		if (m_ActiveWorld->m_CurrentState != WorldState::Edit)
		{
			PF_ERROR("Cannot save when in runtime mode");
			return false;
		}

		if (!AssetManager::HasAsset(m_ActiveWorld->GetID()) && !m_ActiveWorld->GetEntities().empty())
		{
			SaveSceneDialouge = true;
			return false;
		}
		{
			ScopeTimer scopeTime(fmt::format("{} Saved", m_ActiveWorld->GetName()));
			SceneSerializer scerelizer(m_ActiveWorld.Get());
			auto assetInfo = AssetManager::GetAssetInfo(m_ActiveWorld->GetID());
			scerelizer.SerilizeText(Application::Get()->GetProject()->GetAssetFileSystemPath(assetInfo.Path).string());
			PF_INFO("{} Saved", m_ActiveWorld->GetName());

		}

		{
			ScopeTimer scopeTime(fmt::format("AssetManager Saved"));
			AssetManager::SaveAllAssets();
			//AssetManager::SaveAssetManager();
		}

		ProjectSerilizer serilizer(Application::Get()->GetProject().Get());
		if (Application::Get()->GetProject()->GetConfig().OnCloseStartWorldEditLastOpen)
			Application::Get()->GetProject()->m_ProjectConfig.StartWorldEdit = m_ActiveWorld->GetID();
		serilizer.SerilizeText(Application::Get()->GetProject()->GetConfig().Project.string());

		return true;

	}
	void Editore3D::NewWorld()
	{
		if (m_ActiveWorld != nullptr)
		{
			if (m_ActiveWorld->GetState() != WorldState::Edit)
				SetWorldEdit();

			bool val = Save();
			if (val == false)
				return;
		}

		m_EditorWorld = Count<World>::Create();
		m_ActiveWorld = m_EditorWorld;
		s_EditorData->PanelManager->SetWorldContext(m_ActiveWorld);
	}
	void Editore3D::OpenWorld(AssetID ID)
	{

		if (m_ActiveWorld != nullptr)
		{
			if (m_ActiveWorld->GetState() != WorldState::Edit)
				SetWorldEdit();

			bool val =Save();
			if (val == false)
				return;
		}
		

		AssetInfo assetInfo = AssetManager::GetAssetInfo(ID);

		auto fullPath = AssetManager::GetAssetFileSystemPath(assetInfo.Path);
		Count<World> world = Count<World>::Create();

		SceneSerializer serializer(world);
		serializer.DeSerilizeText(fullPath.string());

		m_EditorWorld = world;
		m_ActiveWorld = m_EditorWorld;

		s_EditorData->PanelManager->SetWorldContext(m_ActiveWorld);
		s_EditorData->EditorWorkspaceManager->SetWorldContext(m_ActiveWorld);
		SelectionManager::DeselectAll();

	}
	void Editore3D::UI_StatisticsPanel()
	{
		if (!s_EditorData->ShowStatisticsPanel)
			return;

		if (ImGui::Begin("Statistics", &s_EditorData->ShowStatisticsPanel))
		{
			//ImGui::PushStyleColor(ImGuiCol_WindowBg, Colours::Theme::backgroundDark);
			//ImGui::PushStyleColor(ImGuiCol_ChildBg, Colours::Theme::backgroundDark);

			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar("StatisticsTabs", tab_bar_flags))
			{
				Application& app = *Application::Get();

				if (ImGui::BeginTabItem("Renderer"))
				{
					//auto& caps = Renderer::GetCapabilities();
					//ImGui::Text("Vendor: %s", caps.Vendor.c_str());
					//ImGui::Text("Renderer: %s", caps.Device.c_str());
					//ImGui::Text("Version: %s", caps.Version.c_str());
					//ImGui::Separator();
					ImGui::Text("Frame Time: %.2fms\n", FrameTime::GetFrameMS());
					ImGui::Text("FPS: %.2fms\n", FrameTime::GetFrameFPS());

					#if 0
					if (RendererAPI::Current() == RendererAPIType::Vulkan)
					{
						GPUMemoryStats memoryStats = VulkanAllocator::GetStats();
						std::string used = Utils::BytesToString(memoryStats.Used);
						std::string free = Utils::BytesToString(memoryStats.Free);
						ImGui::Text("Used VRAM: %s", used.c_str());
						ImGui::Text("Free VRAM: %s", free.c_str());
						ImGui::Text("Descriptor Allocs: %d", VulkanRenderer::GetDescriptorAllocationCount(Renderer::RT_GetCurrentFrameIndex()));
					}
					#endif
					bool vsync = app.GetWindow()->IsVsync();
					if (ImGui::Checkbox("Vsync", &vsync))
						app.GetWindow()->SetVsync(vsync);

					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Performance"))
				{
					ImGui::Text("Frame Time: %.2fms\n", FrameTime::GetFrameMS());
					ImGui::Text("FPS: %.2fms\n", FrameTime::GetFrameFPS());

					//const auto& perFrameData = app.GetPerformanceProfiler()->GetPerFrameData();
					//for (auto&& [name, time] : perFrameData)
					//{
					//	ImGui::Text("%s: %.3fms\n", name, time);
					//}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Memory"))
				{
					#if PF_TRACK_MEMORY
					const auto& allocStats = Memory::GetAllocationStats();
					const auto& allocStatsMap = Allocator::GetAllocationStats();

					{
						std::string totalAllocatedStr = Utils::String::BytesToString(allocStats.TotalAllocated);
						std::string totalFreedStr = Utils::String::BytesToString(allocStats.TotalFreed);
						std::string totalUsedStr = Utils::String::BytesToString(allocStats.TotalAllocated - allocStats.TotalFreed);

						ImGui::Text("Total allocated %s", totalAllocatedStr.c_str());
						ImGui::Text("Total freed %s", totalFreedStr.c_str());
						ImGui::Text("Current usage: %s", totalUsedStr.c_str());
					}

					ImGui::Separator();

					static std::string searchedString;

					struct MemoryRefEntry
					{
						const char* Category;
						size_t Size;
					};
					std::vector<MemoryRefEntry> sortedEntries;
					sortedEntries.reserve(allocStatsMap.size());
					for (auto& [category, stats] : allocStatsMap)
					{

						sortedEntries.push_back({ category, stats.TotalAllocated - stats.TotalFreed });
					}

					std::sort(sortedEntries.begin(), sortedEntries.end(), [](auto& a, auto& b) { return a.Size > b.Size; });

					for (const auto& entry : sortedEntries)
					{
						std::string usageStr = Utils::String::BytesToString(entry.Size);

						if (const char* slash = strstr(entry.Category, "\\"))
						{
							std::string tag = slash;
							auto lastSlash = tag.find_last_of("\\");
							if (lastSlash != std::string::npos)
								tag = tag.substr(lastSlash + 1, tag.size() - lastSlash);

							ImGui::TextColored(ImVec4(0.3f, 0.4f, 0.9f, 1.0f), "File: %s: %s", tag.c_str(), usageStr.c_str());
						}
						else
						{
							const char* category = entry.Category;
							if (category = strstr(entry.Category, "class"))
								category += 6;
							ImGui::Text("%s: %s", category, usageStr.c_str());

						}
					}
					#else
					ImGui::TextColored(ImVec4(0.9f, 0.35f, 0.3f, 1.0f), "Memory is not being tracked because PF_TRACK_MEMORY is not defined!");
					#endif

					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

			//ImGui::PopStyleColor(2);
		}

		ImGui::End();
	}
	void Editore3D::UI_HandleAssetDrop()
	{
		if (!ImGui::BeginDragDropTarget())
			return;
		auto data = ImGui::AcceptDragDropPayload("asset_payload");
		if (data)
		{
			uint64_t count = data->DataSize / sizeof(AssetID);
			if(count >0)
				SelectionManager::DeselectAll(SelectionContext::Scene);

			for (uint64_t i = 0; i < count; i++)
			{
				AssetID assetHandle = *(((AssetID*)data->Data) + i);
				

				if (!AssetManager::HasAsset(assetHandle))
					continue;
				const AssetInfo& info = AssetManager::GetAssetInfo(assetHandle);

				// We can't really support dragging and dropping worlds when we're dropping multiple assets
				if (count == 1 && info.Type == AssetType::World)
				{
					OpenWorld(info.ID);
					break;
				}
				if (info.Type == AssetType::MeshSourceFile)
				{
					s_EditorData->CreateNewMeshPopupData.MeshToCreate = AssetManager::GetAsset<MeshSource>(info.ID);
					UI_ShowCreateNewMeshPopup();
				}
				else if (info.Type == AssetType::Mesh)
				{
					Count<Mesh> mesh = AssetManager::GetAsset<Mesh>(info.ID);
					Entity entity = m_ActiveWorld->CreateEntity(info.GetName());
					entity.AddComponent<MeshComponent>().SetMesh(mesh->GetID());
					SelectionManager::DeselectAll();
					SelectionManager::Select(SelectionContext::Scene, entity.GetUUID());
				}
				else if (info.Type == AssetType::DynamicMesh)
				{
					Count<DynamicMesh> mesh = AssetManager::GetAsset<DynamicMesh>(info.ID);
					Entity rootEntity = m_ActiveWorld->CreateEntity(mesh);
					SelectionManager::Select(SelectionContext::Scene, rootEntity.GetUUID());
				}
				else if (info.Type == AssetType::Prefab)
				{
					Count<Prefab> prefab = AssetManager::GetAsset<Prefab>(info.ID);
					m_ActiveWorld->CreateEntity(info.GetName(), prefab, TransformComponent());
				}
			}
		}
		ImGui::EndDragDropTarget();

	}
	void Editore3D::UI_SaveWorldAs()
	{
		UI::ShowMessageBox("Save As World", [this]()
			{
				ImGui::Text(Project::GetActive()->GetProjectDirectory().filename().string().c_str());
				UI::AttributeInputText("WorldPath", s_EditorData->CreateNewWorldPopupData.CreateWorldFile);
				PF_CORE_ASSERT(s_EditorData->CreateNewWorldPopupData.WorldToSaveAs);

				ImGui::Separator();
				if (ImGui::Button("Create"))
				{
					std::filesystem::path savedPath = Project::GetActive()->GetAssetDirectory() / s_EditorData->CreateNewWorldPopupData.CreateWorldFile += Utils::GetAssetExtensionString(AssetType::World);

					savedPath = FileSystem::GenerateUniqueFileName(savedPath);

					if (!FileSystem::Exists(savedPath.parent_path()))
						FileSystem::CreateDirectory(savedPath.parent_path());

					SceneSerializer sereilizer(s_EditorData->CreateNewWorldPopupData.WorldToSaveAs);
					sereilizer.SerilizeText(savedPath.string());
					if (s_EditorData->CreateNewWorldPopupData.SaveAssetManager && !AssetManager::HasAsset(s_EditorData->CreateNewWorldPopupData.WorldToSaveAs))
					{
						auto asset = s_EditorData->CreateNewWorldPopupData.WorldToSaveAs.As<Asset>();
						AssetManager::NewAsset(asset, savedPath);
					}
					SaveSceneDialouge = false;
					PF_INFO("World {} saved as");
				}
			});

	}
	void Editore3D::UI_ShowCreateNewMeshPopup()
	{
		UI::ShowMessageBox("Create New Mesh", [this]()
			{
				static bool dynamicMesh = false;
				static bool doGenerateColliders = true;

				UI::AttributeBool("Dynamic", dynamicMesh);
				UI::AttributeBool("Generate Colliders", doGenerateColliders, "Controls whether physics components (collider and rigid body) will be added to the newly created entity.");
				ImGui::Separator();
				ImGui::Text(Project::GetActive()->GetProjectDirectory().filename().string().c_str());
				UI::AttributeInputText("MeshPath", s_EditorData->CreateNewMeshPopupData.CreateMeshFilenameBuffer);

				

				PF_CORE_ASSERT(s_EditorData->CreateNewMeshPopupData.MeshToCreate);
				if (ImGui::Button("Create"))
				{
					std::filesystem::path savedPath = Project::GetActive()->GetAssetDirectory() / s_EditorData->CreateNewMeshPopupData.CreateMeshFilenameBuffer;
					
					if (!dynamicMesh)
						savedPath += Utils::GetAssetExtensionString(AssetType::Mesh);
					else
						savedPath += Utils::GetAssetExtensionString(AssetType::DynamicMesh);

					if (!FileSystem::Exists(savedPath.parent_path()))
						FileSystem::CreateDirectory(savedPath.parent_path());

					savedPath = FileSystem::GenerateUniqueFileName(savedPath);

					if (!s_EditorData->CreateNewMeshPopupData.MeshToCreate->GetSubMeshes().empty())
					{

						SelectionManager::DeselectAll(SelectionContext::Scene);

						if (!dynamicMesh)
						{
							Count<Mesh> mesh = AssetManager::NewAsset<Mesh>(savedPath, s_EditorData->CreateNewMeshPopupData.MeshToCreate);
							auto entity = s_EditorData->CreateNewMeshPopupData.TargetEntity;

							if (!entity)
							{
								entity = m_ActiveWorld->CreateEntity(AssetManager::GetAssetInfo(mesh).GetName());
							}

							entity.GetorCreateComponent<MeshComponent>().SetMesh(mesh->GetID());
							SelectionManager::Select(SelectionContext::Scene, entity.GetUUID());
							if (doGenerateColliders && !entity.HasComponent<MeshColliderComponent>())
							{
								auto& colliderComponent = entity.GetorCreateComponent<MeshColliderComponent>();
								Count<MeshCollider> colliderAsset = PhysicsEngine::GetOrCreateColliderAsset(entity, colliderComponent);
								colliderComponent.ColliderID = colliderAsset->GetID();
								colliderComponent.SubMeshIndex = 0;
								colliderComponent.UseSharedShape = colliderAsset->AlwaysShareShape;
								entity.GetorCreateComponent<RigidBodyComponent>();

							}

						}
						else
						{
							Count<DynamicMesh> mesh = AssetManager::NewAsset<DynamicMesh>(savedPath, s_EditorData->CreateNewMeshPopupData.MeshToCreate);
							auto entity = s_EditorData->CreateNewMeshPopupData.TargetEntity;
							if (!entity)
							{
								entity = m_ActiveWorld->CreateEntity(mesh, doGenerateColliders);
								SelectionManager::Select(SelectionContext::Scene, entity.GetUUID());
							}
						}
					}

					s_EditorData->CreateNewMeshPopupData = {};
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();

				if (ImGui::Button("Cancel"))
				{
					s_EditorData->CreateNewMeshPopupData = {};
					ImGui::CloseCurrentPopup();
				}
			});
	}
	
	void Editore3D::SetActiveWorld(Count<World> world)
	{
	}
	void Editore3D::PlayWorld() {
		SelectionManager::DeselectAll();
		m_ActiveWorld = World::Copy(m_EditorWorld);
		s_DetachPlayer = false;

		m_ActiveWorld->m_CurrentState = WorldState::Play;
		if (s_EditorData->ClearLogOnPlay)
			Log::Logs.clear();
		m_ActiveWorld->StartRuntime();
		s_EditorData->PanelManager->SetWorldContext(m_ActiveWorld);
		s_EditorData->EditorWorkspaceManager->SetWorldContext(m_ActiveWorld);
	}
	void Editore3D::SimulateWorld() {

		s_DetachPlayer = false;
		m_ActiveWorld->m_CurrentState = WorldState::Simulate;
	}
	void Editore3D::SetWorldEdit() 
	{
		SelectionManager::DeselectAll();

		//s_EditorData->GuizmoType = 0;
		m_ActiveWorld->EndRuntime();
		m_ActiveWorld = m_EditorWorld;
		s_EditorData->PanelManager->SetWorldContext(m_ActiveWorld);
		s_EditorData->EditorWorkspaceManager->SetWorldContext(m_ActiveWorld);
		s_DetachPlayer = false;
	}
	void Editore3D::PauseWorld() {
		m_ActiveWorld->m_CurrentState = WorldState::Pause;
	}
}
