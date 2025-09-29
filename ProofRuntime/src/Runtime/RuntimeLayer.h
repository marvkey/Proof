#pragma once
#include "Proof/Core/Layer.h"
#include "Proof/Scene/Camera/EditorCamera.h"
#include "Proof/Input/ElevatedInputSystem/InputTypes.h"
#include "Proof/Asset/AssetTypes.h"

#include "Proof/Core/FrameTime.h"
namespace Proof
{
	class RuntimeLayer : public Layer {
	public:
		virtual void OnEvent(Event& e);
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(FrameTime DeltaTime) override;

	private:
		bool InputBindElevatedDelegate(const class ElevatedInputKeyParams& key);
		void OnWorldTransition(AssetID ID);
		void DrawDebugStats();
	private:
		Count<class RenderPass> m_RenderPass;
		Count<class Renderer2D> m_Renderer2D;
		bool m_WindowResize = false;
		Count<class World> m_World;
		uint32_t m_PlayerCount = 1;
		Count<class WorldRenderer> m_WorldRenderer;
		Count< class ElevatedInputDeviceManager> m_InputManager;
		class EditorCamera m_Camera;
		bool m_ShowDebugStats;
		Count<class RenderCommandBuffer> m_CommandBuffer;
	};
}
