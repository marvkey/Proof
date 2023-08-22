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
	EditorCamera s_Editorcamera{ 100,100 };

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
		m_World = Count<World>::Create();
		//resize(m_PlayerCount);
		auto startworld = Application::Get()->GetProject()->GetConfig().StartWorld;
		m_WorldRenderer = Count<WorldRenderer>::Create(m_World, Application::Get()->GetWindow()->GetWidth(), Application::Get()->GetWindow()->GetHeight());

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
	void RuntimeLayer::OnUpdate(FrameTime DeltaTime)
	{
		PF_PROFILE_FUNC();
		m_WorldRenderer->ShadowSetting.SoftShadows = false;
		m_World->OnUpdateRuntime(DeltaTime);
		uint32_t windowWIdth = Application::Get()->GetWindow()->GetWidth();
		uint32_t windowHeight = Application::Get()->GetWindow()->GetHeight();
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
		s_Editorcamera.OnUpdate(DeltaTime, (uint32_t)windowWIdth, (uint32_t)windowHeight);
		m_WorldRenderer->Render(s_Editorcamera.m_Projection, s_Editorcamera.m_View,GlmVecToProof( s_Editorcamera.m_Positon), s_Editorcamera.m_FarPlane, s_Editorcamera.m_FarPlane, viewPort, scissor,RenderSettings());
	}

	void RuntimeLayer::OnEvent(Event& e) 
	{
		EventDispatcher dispatcher(e);
		InputManager::OnEvent(e);
		dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& e) {
			m_WindowResize = true;
		});
	}
}
