#include "RuntimeLayer.h"
#include "Proof/Core/Profile.h"
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
#include "Proof/Renderer/RenderPass.h"
#include "Proof/Renderer/GraphicsPipeLine.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/VertexArray.h"
#include "Proof/Renderer/CommandBuffer.h"
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/RenderMaterial.h"
#include "Proof/Renderer/Renderer2D.h"	
#include "Proof/Core/Application.h"
#include "Proof/Project/Project.h"
#include "Proof/Renderer/Font.h"
#include "Proof/Scripting/ScriptWorld.h"

#include "Proof/Input/ElevatedInputSystem/ElevatedInputDevices/ElevatedInputDeviceManager.h"

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
	AssetID changeWorldID = 0;
	bool changeWorld = false;
	void RuntimeLayer::OnAttach()
	{
		PF_PROFILE_FUNC();
		m_Renderer2D = Count<Renderer2D>::Create("Runtime");
		m_World = Count<World>::Create();
		{
			auto startworld = Application::Get()->GetProject()->GetConfig().StartWorld;
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
		}
		m_World->SetWorldTransitionCallback([this](AssetID id) { changeWorld = true;
		changeWorldID = id; });

		m_WorldRenderer = Count<WorldRenderer>::Create();
		m_CommandBuffer = RenderCommandBuffer::CreateFromSwapChain("Runtime");
		//m_CommandBuffer = RenderCommandBuffer::Create("Runtime");

		auto frameBuffer = FrameBuffer::CreateSwapChainFrameBuffer("Runtime");

		Count<GraphicsPipeline> Pipeline;
		{
			Count<VertexArray> quadVertexArray = VertexArray::Create({ sizeof(QuadVertex) });
			quadVertexArray->AddData(0, DataType::Vec3, offsetof(QuadVertex, QuadVertex::Position));
			quadVertexArray->AddData(1, DataType::Vec2, offsetof(QuadVertex, QuadVertex::TexCoord));

			
			GraphicsPipelineConfiguration config;
			config.DebugName = "Runtime";
			config.Attachments = { Application::Get()->GetWindow()->GetSwapChain()->GetColorFormat()};
			config.CullMode = CullMode::None;
			config.WriteDepth = false;
			//config.DepthTest = false;
			config.DepthCompareOperator = DepthCompareOperator::GreaterOrEqual;
			config.Shader = Renderer::GetShader("TextPass");
			config.VertexArray = quadVertexArray;

			Pipeline = GraphicsPipeline::Create(config);
		}

		{
			RenderPassConfig config;
			config.DebugName = "Runtime RenderPass";
			config.Pipeline = Pipeline;
			config.TargetFrameBuffer = frameBuffer;

			m_RenderPass = RenderPass::CreateSwapChain("Runtime RenderPass",Pipeline);
		}

		m_WorldRenderer->ShadowSetting.SoftShadows = false;
		m_InputManager = Count<ElevatedInputDeviceManager>::Create();
		m_InputManager->OnEventDelegate.Bind<&RuntimeLayer::InputBindElevatedDelegate>(this);

		m_World->StartRuntime();

		Application::Get()->GetWindow()->SetWindowInputEvent(true);
		m_WorldRenderer->SetViewportSize(Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight());
	}
	void RuntimeLayer::OnDetach()
	{
		PF_PROFILE_FUNC();

		if(m_World)
			m_World->EndRuntime();
	}
	void RuntimeLayer::OnUpdate(FrameTime DeltaTime)
	{
		PF_PROFILE_FUNC();
		m_InputManager->OnUpdate(DeltaTime);

		if (m_World->HasWorldCamera())
		{
			m_Camera.SetActive(false);
			m_World->OnUpdateRuntime(DeltaTime);
			m_World->OnRenderRuntime(m_WorldRenderer, DeltaTime);
		}
		else
		{
			m_Camera.SetActive(true);
			m_Camera.SetViewportSize(Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight());
			m_World->OnRenderEditor(m_WorldRenderer,DeltaTime,m_Camera);
		}
		if (Input::IsKeyClicked(KeyBoardKey::F3))
			Math::ChangeBool(m_ShowDebugStats);

		if (m_ShowDebugStats)
			DrawDebugStats();

		auto finalImage = m_WorldRenderer->GetExternalCompositePassFrameBuffer()->GetOutput(0);
		if (finalImage)
		{
			m_RenderPass->SetInput("u_Texture", finalImage);
		#if	1
			

			Renderer::BeginCommandBuffer(m_CommandBuffer);

			Renderer::BeginRenderPass(m_CommandBuffer, m_RenderPass,true);
			Renderer::SubmitFullScreenQuad(m_CommandBuffer, m_RenderPass);
			Renderer::EndRenderPass(m_RenderPass);

			Renderer::EndCommandBuffer(m_CommandBuffer);
			Renderer::SubmitCommandBuffer(m_CommandBuffer);
		#endif
		}

		if (changeWorld)
		{
			OnWorldTransition(changeWorldID);
			changeWorld = false;
		}
		
		
	}

	bool RuntimeLayer::InputBindElevatedDelegate(const ElevatedInputKeyParams& key)
	{
		return m_World->OnElevatedKeyEvent(key);
		return false;
	}

	void RuntimeLayer::OnWorldTransition(AssetID ID)
	{

		auto startworld = ID;
		if (AssetManager::HasAsset(startworld))
		{
			m_World->EndRuntime();

			Count<World> newWorld = Count<World>::Create();

			auto Info = AssetManager::GetAssetInfo(startworld);
			SceneSerializer scerelizer(newWorld.Get());
			auto path = Application::Get()->GetProject()->GetAssetFileSystemPath(Info.Path);
			if (scerelizer.DeSerilizeText(path.string()) == true)
			{
				AssetManager::LoadMultipleAsset(scerelizer.GetAssetLoadID());
			}


			m_World = newWorld;
			m_World->SetWorldTransitionCallback([this](AssetID id) 
				{ 
				changeWorld = true;
				changeWorldID = id;
				});
			m_World->StartRuntime();

		}
	}
	void DrawString(Count<Renderer2D> renderer2D,const std::string& string, const glm::vec2& position, const glm::vec4& color, float size)
	{
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(size));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, -0.2f }) * scale;
		TextParams params;
		params.Color = color;
		renderer2D->DrawString(string, Font::GetDefault(), params,transform);
	}

	void RuntimeLayer::DrawDebugStats()
	{
		m_Renderer2D->SetTargetFrameBuffer(m_WorldRenderer->GetExternalCompositePassFrameBuffer());
		Renderer2DContextSettings settings;
		settings.RenderOnTop = true;
		m_Renderer2D->BeginContext(glm::ortho(0.0f, (float)m_WorldRenderer->GetScreenData().FullResolution.x, 0.0f, (float)m_WorldRenderer->GetScreenData().FullResolution.y), glm::mat4(1.0f),Vector(0), settings);

		// Add font size to this after each line
		float y = 30.0f;

		float fontSize = 25.0f;
		/*
		if (false)
		{
			const auto& perFrameData = app.GetProfilerPreviousFrameData();
			for (const auto& [name, time] : perFrameData)
			{
				DrawString(renderer2D,fmt::format("{}: {:.3f} ms", name, time), { 30.0f, y }, glm::vec4(1.0f), fontSize);
				y += fontSize;
			}
		}
		*/

		fontSize = 30.0f;
		DrawString(m_Renderer2D,fmt::format("{} fps", (uint32_t)FrameTime::GetFrameFPS()), { 30.0f, y }, glm::vec4(1.0f), fontSize);
		y += fontSize;

		//DrawString(fmt::format("{:.2f} ms ({:.2f}ms GPU)", m_FrameTime, m_GPUTime), { 30.0f, y }, glm::vec4(1.0f), fontSize);
		//y += fontSize;

		//DrawString(fmt::format("Render Thread {:.2f} ms ({:.2f}ms wait)", m_PerformanceTimers.RenderThreadWorkTime, m_PerformanceTimers.RenderThreadWaitTime), { 30.0f, y }, glm::vec4(1.0f), fontSize);
		//y += fontSize;
		//DrawString(fmt::format("Main Thread {:.2f} ms ({:.2f}ms wait)", m_PerformanceTimers.MainThreadWorkTime, m_PerformanceTimers.MainThreadWaitTime), { 30.0f, y }, glm::vec4(1.0f), fontSize);
		//y += fontSize;
		
		DrawString(m_Renderer2D,fmt::format("{} entities", (uint32_t)m_World->GetEntities().size()), { 30.0f, y }, glm::vec4(1.0f), fontSize);
		y += fontSize;
		if (m_World->GetScriptWorld())
		{
			//DrawString(m_Renderer2D, fmt::format("{} script entities", (uint32_t)m_World->GetScriptWorld().size()), {30.0f, y}, glm::vec4(1.0f), fontSize);
			//y += fontSize;
		}
		DrawString(m_Renderer2D,fmt::format("{} ({})", m_World->GetName(), (uint64_t)m_World->GetID()), { 30.0f, y }, glm::vec4(1.0f), fontSize * 0.8f);
		y += fontSize * 0.8f;

		m_Renderer2D->EndContext();
	}

	void RuntimeLayer::OnEvent(Event& e) 
	{
		PF_PROFILE_FUNC();

		EventDispatcher dispatcher(e);
		InputManager::OnEvent(e);
		m_InputManager->OnEvent(e);
		dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& e) 
		{

			m_WorldRenderer->SetViewportSize(e.GetWhidt(), e.GetHeight());
			return false;
		});

	}
}
