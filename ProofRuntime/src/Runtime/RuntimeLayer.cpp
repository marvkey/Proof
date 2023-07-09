#include "RuntimeLayer.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/SceneSerializer.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Input/Controller.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Input/InputManager.h"
#include "Proof/Input/Mouse.h"
#include "Proof/Core/Application.h"
#include "Proof/Renderer/SwapChain.h"
namespace Proof {

		/*
		{
			//https://github.com/SaschaWillems/Vulkan/blob/master/examples/negativeviewportheight/negativeviewportheight.cpp
			//https://www.saschawillems.de/blog/2019/03/29/flipping-the-vulkan-viewport/
			Viewport viewPort;
			viewPort.X = 0.0f;
			viewPort.Y = (float)m_WorldRenderers->GetFrameHeight();
			viewPort.Width = m_WorldRenderers->GetFrameWidth();
			viewPort.Height = -(float)m_ScreenFrameBuffer->GetFrameHeight();
			viewPort.MinDepth = 0;
			viewPort.MaxDepth = 1;

			ViewportScissor scissor;
			scissor.Offset = { 0,0 };
			scissor.Extent = { (float)m_ScreenFrameBuffer->GetFrameWidth(),(float)m_ScreenFrameBuffer->GetFrameHeight() };
		}
		*/
	//Count<FrameBuffer> m_ScreenImage;
	void RuntimeLayer::OnAttach()
	{
		m_PlayerCount = 1;
		m_PlayerCount += Application::Get()->GetWindow()->GetControllers().size();
		m_World = Count<World>::Create();
		m_WorldRenderers.resize(4);
		//resize(m_PlayerCount);
		auto startworld = Application::Get()->GetProject()->GetConfig().StartWorld;
		for (int i = 0; i < 4; i++)
			m_WorldRenderers[i] = Count<WorldRenderer>::Create(m_World, Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight());

		if (AssetManager::HasAsset(startworld))
		{
			auto Info = AssetManager::GetAssetInfo(startworld);
			SceneSerializer scerelizer(m_World.Get());
			auto path = Application::Get()->GetProject()->GetAssetFileSystemPath(Info.Path);
			if (scerelizer.DeSerilizeText(path.string()) == true)
			{
				AssetManager::LoadMultipleAsset(scerelizer.GetAssetLoadID());
			}
		}

		m_World->StartRuntime();
		Application::Get()->GetWindow()->SetWindowInputEvent(true);

	}
	void RuntimeLayer::OnDetach()
	{
		if(m_World)
			m_World->EndRuntime();
		m_World = nullptr;
	}
	bool enableStart = false;
	void RuntimeLayer::OnUpdate(FrameTime DeltaTime)
	{
		PF_PROFILE_FUNC();

		m_DrawFirstPlayer = false;

		if (enableStart)
		{
			m_World->StartRuntime();
		}
		uint32_t windowWIdth = Application::Get()->GetWindow()->GetWidth();
		uint32_t windowHeight = Application::Get()->GetWindow()->GetHeight();
		if (Input::IsKeyClicked(KeyBoardKey::P))
		{
			if (m_World->GetID() == 9731897389791312)
			{
				m_World->EndRuntime();

				m_World = Count<World>::Create();
				m_PlayerCount = 1;
				m_PlayerCount += Application::Get()->GetWindow()->GetControllers().size();
				for (int i = 0; i < 4; i++)
					m_WorldRenderers[i]->SetContext(m_World);

				auto Info = AssetManager::GetAssetInfo(13308300816728474787);
				SceneSerializer scerelizer(m_World.Get());
				auto path = Application::Get()->GetProject()->GetAssetFileSystemPath(Info.Path);
				if (scerelizer.DeSerilizeText(path.string()) == true)
				{
					AssetManager::LoadMultipleAsset(scerelizer.GetAssetLoadID());
				}
				enableStart = true;
				//m_World->StartRuntime();
				return;
			}
		}

		if (Mouse::IsMouseCaptured() == false && m_World->GetID() == 13308300816728474787)
		{
			if (Input::IsKeyClicked(KeyBoardKey::Q))
			{
				Application::Get()->ShutDown();
			}

			if (Input::IsKeyClicked(KeyBoardKey::N))
			{
				m_World->EndRuntime();

				m_World = Count<World>::Create();
				m_PlayerCount = 1;
				m_PlayerCount += Application::Get()->GetWindow()->GetControllers().size();
				for (int i = 0; i < 4; i++)
					m_WorldRenderers[i]->SetContext(m_World);
				
				auto Info = AssetManager::GetAssetInfo(13308300816728474787);
				SceneSerializer scerelizer(m_World.Get());
				auto path = Application::Get()->GetProject()->GetAssetFileSystemPath(Info.Path);
				if (scerelizer.DeSerilizeText(path.string()) == true)
				{
					AssetManager::LoadMultipleAsset(scerelizer.GetAssetLoadID());
				}
				enableStart = true;
				return;
			}
		}

		if (m_World->EnableRestart == true)
		{
			uint64_t id = m_World->GetID();
			m_World->EndRuntime();
			m_World = Count<World>::Create();
			m_PlayerCount = 1;
			m_PlayerCount += Application::Get()->GetWindow()->GetControllers().size();
			for (int i = 0; i < 4; i++)
				m_WorldRenderers[i]->SetContext(m_World);

			auto Info = AssetManager::GetAssetInfo(id);
			SceneSerializer scerelizer(m_World.Get());
			auto path = Application::Get()->GetProject()->GetAssetFileSystemPath(Info.Path);
			if (scerelizer.DeSerilizeText(path.string()) == true)
			{
				AssetManager::LoadMultipleAsset(scerelizer.GetAssetLoadID());
			}
			m_World->EnableRestart = false;
			enableStart = true;
			return;
		}
		if (m_WindowResize == true)
		{
			for (int i = 0; i < 4; i++)
				m_WorldRenderers[i]->Resize({ Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight() });
		}
		if (m_World->GetState() == WorldState::Pause)
		{
			m_World->ForEachEnitityWith<PlayerInputComponent>([&](Entity entity) {
				if (entity.ForceGetComponent<PlayerInputComponent>().InputPlayer != Players::Player0)
					return;
				Viewport viewPort;
				viewPort.X = 0.0f;
				viewPort.Y = (float)windowHeight;
				viewPort.Width = windowWIdth;
				viewPort.Height = -(float)windowHeight;
				viewPort.MinDepth = 0;
				viewPort.MaxDepth = 1;

				ViewportScissor scissor;
				scissor.Offset = { 0,0 };
				scissor.Extent = { (float)windowWIdth,(float)windowHeight };
				auto location = m_World->GetWorldLocation(entity);
				Vector rotation;
				if (entity.GetCamera().GetComponent<CameraComponent>()->UseLocalRotation)
					rotation = entity.GetCamera().GetComponent<TransformComponent>()->Rotation;
				else
					rotation = m_World->GetWorldRotation(entity);
				entity.GetCamera().GetComponent<CameraComponent>()->Width = windowWIdth;
				entity.GetCamera().GetComponent<CameraComponent>()->Height = windowHeight;
				entity.GetCamera().GetComponent<CameraComponent>()->CalculateProjection(location, rotation);
				if (!entity.HasComponent<PlayerHUDComponent>())
					m_WorldRenderers[0]->Render(*entity.GetCamera().GetComponent<CameraComponent>(), location, RenderSettings(), nullptr);
				else
					m_WorldRenderers[0]->Render(*entity.GetCamera().GetComponent<CameraComponent>(), location, RenderSettings(), entity.GetComponent<PlayerHUDComponent>()->HudTable);
				m_World->OnUpdateRuntime(DeltaTime);
				m_DrawFirstPlayer = true;
			});
			return;
		}

		if (m_World->GetNumComponents<PlayerInputComponent>()>0)
		{
			//m_WorldRenderer->BeginContext();
			//m_World->ForEachEnitityWith<PlayerInputComponent>([&](Entity entity) { 
			//	if (m_PlayerCount == 2)
			//	{
			//	}
			//});
			//m_WorldRenderer->EndContext();
			//if (m_PlayerCount == 1)
			//{
			//	m_WorldRenderers[0]->Render()
			//}
			m_World->ForEachEnitityWith<PlayerInputComponent>([&](Entity entity) { 
				RenderScreenPlayerCount(entity, m_World);
			});

		}
		else
		{
			Viewport viewPort;
			viewPort.X = 0.0f;
			//viewPort.Y = (float)windowHeight;
			viewPort.Width = windowWIdth;
			viewPort.Height = -(float)windowHeight;
			viewPort.MinDepth = 0;
			viewPort.MaxDepth = 1;

			ViewportScissor scissor;
			scissor.Offset = { 0,0 };
			scissor.Extent = { (float)windowWIdth,(float)windowHeight };
			auto entity = m_World->GetWorldCameraEntity();
			auto location = m_World->GetWorldLocation(entity);
			Vector rotation;
			if (entity.GetComponent<CameraComponent>()->UseLocalRotation)
				rotation = entity.GetComponent<TransformComponent>()->Rotation;
			else
				rotation = m_World->GetWorldRotation(entity);
			entity.GetComponent<CameraComponent>()->Width = windowWIdth;
			entity.GetComponent<CameraComponent>()->Height = windowHeight;
			entity.GetComponent<CameraComponent>()->CalculateProjection(location, rotation);
			if (!entity.HasComponent<PlayerHUDComponent>())
				m_WorldRenderers[0]->Render(*entity.GetComponent<CameraComponent>(), location, RenderSettings(), nullptr);
			else
				m_WorldRenderers[0]->Render(*entity.GetComponent<CameraComponent>(), location, RenderSettings(), entity.GetComponent<PlayerHUDComponent>()->HudTable);
			m_DrawFirstPlayer = true;

		}
		if (enableStart == false)
			m_World->OnUpdateRuntime(DeltaTime);
		enableStart = false;
	}

	void RuntimeLayer::OnImGuiDraw(FrameTime DeltaTime)
	{

		PF_PROFILE_FUNC();

		static bool EnableDocking = true;
		SetDocking(&EnableDocking);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

		static bool Open = true;

		ImGui::Begin("View",&Open, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);

		if (m_DrawFirstPlayer)
		{
			ImGui::Image((ImTextureID)m_WorldRenderers[0]->GetImage().SourceImage, { ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		}
		if (m_PlayerCount == 2 && m_DrawFirstPlayer == false)
		{
			ImGui::Image((ImTextureID)m_WorldRenderers[0]->GetImage().SourceImage, { ImGui::GetContentRegionAvail().x / 2,ImGui::GetContentRegionAvail().y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			ImGui::SameLine();
			ImGui::Image((ImTextureID)m_WorldRenderers[1]->GetImage().SourceImage, { ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		}

		if (m_PlayerCount == 3 && m_DrawFirstPlayer == false)
		{
			ImGui::Image((ImTextureID)m_WorldRenderers[0]->GetImage().SourceImage, { ImGui::GetContentRegionAvail().x / 3,ImGui::GetContentRegionAvail().y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			ImGui::SameLine();
			ImGui::Image((ImTextureID)m_WorldRenderers[1]->GetImage().SourceImage, { ImGui::GetContentRegionAvail().x /3,ImGui::GetContentRegionAvail().y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			ImGui::SameLine();
			ImGui::Image((ImTextureID)m_WorldRenderers[2]->GetImage().SourceImage, { ImGui::GetContentRegionAvail().x / 3,ImGui::GetContentRegionAvail().y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		}

		if (m_PlayerCount == 4 && m_DrawFirstPlayer == false)
		{
			ImGui::Image((ImTextureID)m_WorldRenderers[0]->GetImage().SourceImage, { ImGui::GetContentRegionAvail().x / 2,ImGui::GetContentRegionAvail().y/2 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			ImGui::SameLine();
			ImGui::Image((ImTextureID)m_WorldRenderers[1]->GetImage().SourceImage, { ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y / 2 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

			ImGui::Image((ImTextureID)m_WorldRenderers[2]->GetImage().SourceImage, { ImGui::GetContentRegionAvail().x / 2,ImGui::GetContentRegionAvail().y  }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			ImGui::SameLine();
			ImGui::Image((ImTextureID)m_WorldRenderers[3]->GetImage().SourceImage, { ImGui::GetContentRegionAvail().x,ImGui::GetContentRegionAvail().y  }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		}

		if (m_PlayerCount == 1 && m_DrawFirstPlayer == false)
		{
			ImGui::Image((ImTextureID)m_WorldRenderers[0]->GetImage().SourceImage, { ImGui::GetContentRegionAvail().x ,ImGui::GetContentRegionAvail().y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		}
		ImGui::End();
		ImGui::PopStyleVar(); 
	}

	void RuntimeLayer::SetDocking(bool* p_open)
	{
		// Note: Switch this to true to enable dockspace
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
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
		
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		ImGui::End();
	}

	void RuntimeLayer::OnEvent(Event& e) 
	{
		EventDispatcher dispatcher(e);
		InputManager::OnEvent(e);
		dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& e) {
			m_WindowResize = true;
		});
	}
	int num = 0;
	void RuntimeLayer::RenderScreenPlayerCount(Entity entity, Count<World> world)
	{
		uint32_t windowWIdth = Application::Get()->GetWindow()->GetWidth();
		uint32_t windowHeight = Application::Get()->GetWindow()->GetHeight();
		if (entity.ForceGetComponent<PlayerInputComponent>().InputPlayer == Players::Player0)
		{
			auto camera = entity.GetCamera().GetComponent<CameraComponent>();
			auto location = world->GetWorldLocation(entity.GetCamera());

			Vector rotation;
			if (camera->UseLocalRotation)
				rotation = entity.GetCamera().GetComponent<TransformComponent>()->Rotation;
			else
				rotation = world->GetWorldRotation(entity.GetCamera());
			camera->Width = windowWIdth / 2;
			camera->Height = windowHeight / 2;
			camera->CalculateProjection(location, rotation);

			Viewport viewPort;
			viewPort.X = 0.0f;
			viewPort.Y = (float)(windowHeight)-0;
			viewPort.Width = (float)(windowWIdth / 2);
			viewPort.Height = -(float)(windowHeight);
			viewPort.MinDepth = 0;
			viewPort.MaxDepth = 1;

			ViewportScissor scissor;
			scissor.Offset = { 0,0 };
			scissor.Extent = { (float)(windowWIdth / 2),(float)windowHeight };

			//m_WorldRenderers[0]->AddRender(camera->Projection,
			//	camera->View,
			//	location,
			//	viewPort,
			//	scissor,
			//	RenderSettings(),
			//	true,
			//	entity.GetComponent<PlayerHUDComponent>()->HudTable);
			m_WorldRenderers[0]->Render(*camera, location, RenderSettings(), entity.GetComponent<PlayerHUDComponent>()->HudTable);

		}
		if (entity.ForceGetComponent<PlayerInputComponent>().InputPlayer == Players::Player1)
		{
			auto camera = entity.GetCamera().GetComponent<CameraComponent>();
			auto location = world->GetWorldLocation(entity.GetCamera());

			Vector rotation;
			if (camera->UseLocalRotation)
				rotation = entity.GetCamera().GetComponent<TransformComponent>()->Rotation;
			else
				rotation = world->GetWorldRotation(entity.GetCamera());
			camera->Width = windowWIdth / 2;
			camera->Height = windowHeight / 2;
			camera->CalculateProjection(location, rotation);

			Viewport viewPort;
			viewPort.X = (float)(windowWIdth / 4);
			viewPort.Y = (float)(windowHeight)-0;
			viewPort.Width = (float)(windowWIdth / 2);
			viewPort.Height = -(float)(windowHeight);
			viewPort.MinDepth = 0;
			viewPort.MaxDepth = 1;

			ViewportScissor scissor;
			scissor.Offset = { (float)(windowWIdth / 4),0 };
			scissor.Extent = { (float)(windowWIdth / 2),(float)windowHeight };
			m_WorldRenderers[1]->Render(*camera, location, RenderSettings(), entity.GetComponent<PlayerHUDComponent>()->HudTable);
			
			//m_WorldRenderers[0]->Render(*camera, location, viewPort, scissor, RenderSettings(), false, entity.GetComponent<PlayerHUDComponent>()->HudTable);
		}

		if (entity.ForceGetComponent<PlayerInputComponent>().InputPlayer == Players::Player2)
		{
			auto camera = entity.GetCamera().GetComponent<CameraComponent>();
			auto location = world->GetWorldLocation(entity.GetCamera());

			Vector rotation;
			if (camera->UseLocalRotation)
				rotation = entity.GetCamera().GetComponent<TransformComponent>()->Rotation;
			else
				rotation = world->GetWorldRotation(entity.GetCamera());
			camera->Width = windowWIdth / 2;
			camera->Height = windowHeight / 2;
			camera->CalculateProjection(location, rotation);

			Viewport viewPort;
			viewPort.X = (float)(windowWIdth / 4);
			viewPort.Y = (float)(windowHeight)-0;
			viewPort.Width = (float)(windowWIdth / 2);
			viewPort.Height = -(float)(windowHeight);
			viewPort.MinDepth = 0;
			viewPort.MaxDepth = 1;

			ViewportScissor scissor;
			scissor.Offset = { (float)(windowWIdth / 4),0 };
			scissor.Extent = { (float)(windowWIdth / 2),(float)windowHeight };
			m_WorldRenderers[2]->Render(*camera, location, RenderSettings(), entity.GetComponent<PlayerHUDComponent>()->HudTable);

			//m_WorldRenderers[0]->Render(*camera, location, viewPort, scissor, RenderSettings(), false, entity.GetComponent<PlayerHUDComponent>()->HudTable);
		}
		if (entity.ForceGetComponent<PlayerInputComponent>().InputPlayer == Players::Player3)
		{
			auto camera = entity.GetCamera().GetComponent<CameraComponent>();
			auto location = world->GetWorldLocation(entity.GetCamera());

			Vector rotation;
			if (camera->UseLocalRotation)
				rotation = entity.GetCamera().GetComponent<TransformComponent>()->Rotation;
			else
				rotation = world->GetWorldRotation(entity.GetCamera());
			camera->Width = windowWIdth / 2;
			camera->Height = windowHeight / 2;
			camera->CalculateProjection(location, rotation);

			Viewport viewPort;
			viewPort.X = (float)(windowWIdth / 4);
			viewPort.Y = (float)(windowHeight)-0;
			viewPort.Width = (float)(windowWIdth / 2);
			viewPort.Height = -(float)(windowHeight);
			viewPort.MinDepth = 0;
			viewPort.MaxDepth = 1;

			ViewportScissor scissor;
			scissor.Offset = { (float)(windowWIdth / 4),0 };
			scissor.Extent = { (float)(windowWIdth / 2),(float)windowHeight };
			m_WorldRenderers[3]->Render(*camera, location, RenderSettings(), entity.GetComponent<PlayerHUDComponent>()->HudTable);

			//m_WorldRenderers[0]->Render(*camera, location, viewPort, scissor, RenderSettings(), false, entity.GetComponent<PlayerHUDComponent>()->HudTable);
		}
	}
}
