#include "Proofprch.h"
#include "LocalGameMode.h"
#include "../World.h"
#include "../Component.h"
#include "../Entity.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Renderer/VertexArray.h"
#include "Proof/Renderer/GraphicsPipeLine.h"
#include "Proof/Renderer/RenderPass.h"
#include "Proof/Renderer/Renderer.h"	
#include "Proof/Renderer/CommandBuffer.h"	
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/Vertex.h"
#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Renderer/UIRenderer/UIRenderer.h"
#include "Proof/Utils/ContainerUtils.h"
#include "Proof/Scene/Prefab.h"
#include <future>
namespace Proof
{
	void LocalGameMode::Start()
	{
		m_World->ForEachEnitityWith<PlayerStartComponent>([&](Entity& e)
			{
				if ((int)e.GetComponent<PlayerStartComponent>().InputPlayer < m_World->GetRuntimeConfig().PlayerInputCount )
				{
					if (e.GetComponent<PlayerStartComponent>().Player)
					{
						auto prefab = e.GetComponent<PlayerStartComponent>().Player.GetAsset<Prefab>();
						Entity newEntity = m_World->CreateEntity(e.GetName(), prefab, m_World->GetWorldSpaceTransformComponent(e));
						CreatePlayer(newEntity.GetUUID());
						e.GetComponent<PlayerStartComponent>().Used = true;
					}
				}

			});

		if (m_World->GetRuntimeConfig().PlayerInputCount > 1 )
		{
			m_LocalGameModeSate = LocalGameModeState::MultiPlayer;
			if(m_World->GetRuntimeConfig().CameraOptions == RuntimeConfiguration::CameraSettings::PerPlayer)
				GenerateMultiplayerPipeLines();
		}
		else
			m_LocalGameModeSate = LocalGameModeState::SinglePlayer;

		PF_EC_INFO("LocalGame Mode Started {}", m_Players.size());
	}
	void LocalGameMode::End()
	{
	}
	void LocalGameMode::RenderRuntime(Count<class WorldRenderer> renderer, FrameTime time)
	{
		switch (m_LocalGameModeSate)
		{
			case Proof::LocalGameModeState::SinglePlayer:
				RenderRuntimeSinglePlayer(renderer, time);
				break;
			case Proof::LocalGameModeState::MultiPlayer:
				RenderRuntimeMultiPlayer(renderer, time);
				break;
			default:
				break;
		}

		RenderWorldHUD(renderer);
	}
	void LocalGameMode::RenderRuntimeSinglePlayer(Count<class WorldRenderer> renderer, FrameTime time)
	{
		PF_CORE_ASSERT(renderer);

		if (!m_World->HasWorldCamera())
		{
			PF_CORE_ASSERT(false, "No World Camera");
			return;
		}
		auto worldCameraEntity = m_World->GetWorldCameraEntity();
		PF_CORE_ASSERT(worldCameraEntity.HasComponent<CameraComponent>());
		CameraComponent& cameraComp = worldCameraEntity.GetComponent<CameraComponent>();

		SceneCamera sceneCamera;
		//sceneCamera.SetPerspective(cameraComp.FovDeg, renderer->GetScreenData().FullResolution.x, renderer->GetScreenData().FullResolution.y,
		//	cameraComp.NearPlane, cameraComp.FarPlane, GetWorldSpaceLocation(worldCameraEntity),
		//	cameraComp.UseLocalRotation ? worldCameraEntity.GetComponent<TransformComponent>().GetRotationEuler(): GetWorldSpaceRotation(worldCameraEntity));

		sceneCamera.SetData(cameraComp.FovDeg, cameraComp.NearPlane, cameraComp.FarPlane,
			renderer->GetScreenData().FullResolution.x, renderer->GetScreenData().FullResolution.y,
			glm::inverse(cameraComp.UseLocalRotation ? m_World->GetWorldSpaceTransformUsingLocalRotation(worldCameraEntity) : m_World->GetWorldSpaceTransform(worldCameraEntity)));

		m_World->OnRender(renderer, time, sceneCamera, m_World->GetWorldSpaceLocation(worldCameraEntity), cameraComp.NearPlane, cameraComp.FarPlane, cameraComp.FovDeg);
	}
	void LocalGameMode::RenderRuntimeMultiPlayer(Count<class WorldRenderer> renderer, FrameTime time)
	{
		std::vector<Count<Image2D>> images;
		images.resize(4);
		images[0] = Renderer::GetBlackTexture()->GetImage();
		images[1] = Renderer::GetBlackTexture()->GetImage();
		images[2] = Renderer::GetBlackTexture()->GetImage();
		images[3] = Renderer::GetBlackTexture()->GetImage();
		const int maxCount = glm::clamp((int)m_Players.size(), 1, 4);
	#if 1
		int count = 0;


		glm::vec2 newSize = renderer->GetScreenData().FullResolution / (float)maxCount;
		newSize = glm::max(newSize, glm::vec2(500.0f, 500.0f));
		for (auto& player : m_Players)
		{
			if (count == images.size())
				break;
			player.Renderer->SetViewportSize(newSize.x, newSize.y);

			RenderPlayer(player, time);
			images[count] = player.Renderer->GetFinalPassImage();
			count++;
		}

	#else

		glm::vec2 newSize = renderer->GetScreenData().FullResolution / static_cast<float>(maxCount);
		std::atomic<int> count{ 0 }; // To track how many players have been rendered
		std::vector<std::future<void>> futures; // To hold async tasks

		// Lambda function for rendering a single player
		auto renderPlayerTask = [&](LocalMultiplayerPlayer& player)
		{
			// If we exceed the max image count, return early
			int index = count.fetch_add(1);
			if (index >= images.size())
				return;

			// Set the viewport size for the player's renderer
			player.Renderer->SetViewportSize(newSize.x, newSize.y);

			// Render the player
			RenderPlayer(player, time);

			// Get the final pass image and store it in the images vector
			images[index] = player.Renderer->GetFinalPassImage();
		};

		// Launch async tasks for each player
		for (auto& player : m_Players)
		{
			futures.push_back(std::async(std::launch::async, renderPlayerTask, std::ref(player)));
		}

		// Wait for all tasks to complete
		for (auto& future : futures)
		{
			future.get(); // Ensures any exceptions are propagated
		}
	#endif

		Renderer::BeginCommandBuffer(m_CommandBuffer);
		struct PushData
		{
			glm::vec2 ScreenSize;
			int ViewPortCount;

		}PushInfo;

		PushInfo.ScreenSize = renderer->GetScreenData().FullResolution;
		PushInfo.ViewPortCount = m_Players.size();
		m_PlayerGridPass->SetTargetFrameBuffer(renderer->GetExternalCompositePassFrameBuffer());


		m_PlayerGridPass->SetInput("u_Texture", images);

		Renderer::BeginRenderPass(m_CommandBuffer, m_PlayerGridPass, true);
		m_PlayerGridPass->PushData("u_PC", &PushInfo);
		Renderer::SubmitFullScreenQuad(m_CommandBuffer, m_PlayerGridPass);
		Renderer::EndRenderPass(m_PlayerGridPass);

		Renderer::EndCommandBuffer(m_CommandBuffer);
		Renderer::SubmitCommandBuffer(m_CommandBuffer);
	}
	bool LocalGameMode::RenderPlayer(LocalMultiplayerPlayer& player, FrameTime time)
	{
		Entity e = m_World->TryGetEntityWithUUID(player.PlayerUUID);
		if (!e)
			return false ;
		Entity worldCameraEntity = e.GetCamera();

		if (!worldCameraEntity)
			return false;

		Count<WorldRenderer> renderer = player.Renderer;

		PF_CORE_ASSERT(worldCameraEntity.HasComponent<CameraComponent>());
		CameraComponent& cameraComp = worldCameraEntity.GetComponent<CameraComponent>();

		SceneCamera sceneCamera;
		//sceneCamera.SetPerspective(cameraComp.FovDeg, renderer->GetScreenData().FullResolution.x, renderer->GetScreenData().FullResolution.y,
		//	cameraComp.NearPlane, cameraComp.FarPlane, GetWorldSpaceLocation(worldCameraEntity),
		//	cameraComp.UseLocalRotation ? worldCameraEntity.GetComponent<TransformComponent>().GetRotationEuler(): GetWorldSpaceRotation(worldCameraEntity));

		sceneCamera.SetData(cameraComp.FovDeg, cameraComp.NearPlane, cameraComp.FarPlane,
			renderer->GetScreenData().FullResolution.x, renderer->GetScreenData().FullResolution.y,
			glm::inverse(cameraComp.UseLocalRotation ? m_World->GetWorldSpaceTransformUsingLocalRotation(worldCameraEntity) : m_World->GetWorldSpaceTransform(worldCameraEntity)));

		m_World->OnRender(renderer, time, sceneCamera, m_World->GetWorldSpaceLocation(worldCameraEntity), cameraComp.NearPlane, cameraComp.FarPlane, cameraComp.FovDeg);
		
		if (e.HasComponent<PlayerHUDComponent>())
		{
			PlayerHUDComponent& hudComponent = e.GetComponent<PlayerHUDComponent>();

			auto hudTable = hudComponent.HudTable;

			auto& layers = hudTable->GetLayers();

			for (int i = layers.size() - 1; i >= 0; i--) 
			{
				auto& layer = hudTable->GetLayer(i);

				for (int j = layer.GetUIPanels().size() - 1; j >= 0; j--)
				{
					Count<UIPanelInstance> panelInstance = layer.GetUIPanels().at(j);
					UIRenderer::DrawUI(panelInstance->GetInstanceMenu(), renderer->GetRenderer2D(), renderer->GetScreenData().FullResolution.x, renderer->GetScreenData().FullResolution.y);

				}
			}
		}
		
		return true;
	}
	void LocalGameMode::GenerateMultiplayerPipeLines()
	{
		m_Players.clear();
		Count<GraphicsPipeline> Pipeline;
		{
			Count<VertexArray> quadVertexArray = VertexArray::Create({ sizeof(QuadVertex) });
			quadVertexArray->AddData(0, DataType::Vec3, offsetof(QuadVertex, QuadVertex::Position));
			quadVertexArray->AddData(1, DataType::Vec2, offsetof(QuadVertex, QuadVertex::TexCoord));


			GraphicsPipelineConfiguration config;
			config.DebugName = "LocalMultiplayer";
			config.Attachments = { ImageFormat::RGBA32F,ImageFormat::DEPTH32F };
			config.CullMode = CullMode::None;
			config.WriteDepth = false;
			//config.DepthTest = false;
			config.DepthCompareOperator = DepthCompareOperator::GreaterOrEqual;
			config.Shader = Renderer::GetShader("TextPassMultiPlayer");
			config.VertexArray = quadVertexArray;

			Pipeline = GraphicsPipeline::Create(config);
		}

		{
			RenderPassConfig config;
			config.DebugName = "LocalMultiplayer";
			config.Pipeline = Pipeline;
			config.TargetFrameBuffer = nullptr;

			m_PlayerGridPass = RenderPass::Create(config);
		}
		m_CommandBuffer = RenderCommandBuffer::Create("LocalMultiplayer");

		m_World->ForEachEnitityWith<PlayerInputComponent>([&](Entity& e)
			{
				//if (e.GetComponent<PlayerInputComponent>().InputPlayer == Players::None)
					//return;

				LocalMultiplayerPlayer player;
				player.PlayerUUID = e.GetUUID();
				player.Renderer = Count<WorldRenderer>::Create();

				player.Renderer->SetViewportSize(300, 300);
				m_Players.push_back(player);
			});
	}
	void LocalGameMode::CreatePlayer(UUID entityID, Players player)
	{
		if (!m_World->HasEntity(entityID))
			return;

		Entity playerEntity = m_World->GetEntity(entityID);
		if (player == Players::None)
		{
			std::vector<int> players;
			m_World->ForEachEnitityWith<InternalPlayerInputComponent>([&](Entity e)
				{
					players.push_back((int)e.GetComponent<InternalPlayerInputComponent>().GetPlayer());
				});
			Utils::Sort(players, [](int a, int b) { return a < b; });
			auto missingPlayers = Utils::FindMissingNumbers<int>(players);

			if (missingPlayers.empty())
			{
				if (players.empty())
					playerEntity.AddComponent<InternalPlayerInputComponent>().m_InputPlayer = Players::Player0;
				else
					playerEntity.AddComponent<InternalPlayerInputComponent>().m_InputPlayer = (Players) (players.back() + 1);
			}
			else
			{
				playerEntity.AddComponent<InternalPlayerInputComponent>().m_InputPlayer = (Players)(missingPlayers.front() + 1);
			}
		}
		else
		{
			playerEntity.AddComponent<InternalPlayerInputComponent>().m_InputPlayer = player;

		}
	}

	void LocalGameMode::RenderWorldHUD(Count<class WorldRenderer> renderer)
	{
		m_World->ForEachEnitityWith<WorldHUDComponent>([&](Entity& e)
			{
				WorldHUDComponent& worldHUDComponent = e.GetComponent<WorldHUDComponent>();

				auto hudTable = worldHUDComponent.HudTable;

				auto& layers = hudTable->GetLayers();

				for (int i = layers.size() - 1; i >= 0; i--)
				{
					auto& layer = hudTable->GetLayer(i);

					for (int j = layer.GetUIPanels().size() - 1; j >= 0; j--)
					{
						Count<UIPanelInstance> panelInstance = layer.GetUIPanels().at(j);
						UIRenderer::DrawUI(panelInstance->GetInstanceMenu(), renderer->GetRenderer2D(), renderer->GetScreenData().FullResolution.x, renderer->GetScreenData().FullResolution.y);

					}
				}

			});
	}
}

