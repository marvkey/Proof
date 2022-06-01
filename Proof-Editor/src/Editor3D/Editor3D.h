#pragma once
#include "Proof/Core/Layer.h"
#include "glm/glm.hpp"
#include "Proof/ProofCore.h"
#include "MainWindow/SceneHierachyPanel.h"
#include "MainWindow/ContentBrowserPanel.h"
#include "Proof/Scene/SceneSerializer.h"
#include "MainWindow/Panel.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "MainWindow/AssetManagerPanel.h"
#include "MainWindow/InputPanel.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include<vulkan/vulkan.h>
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "MainWindow/PerformancePanel.h"
namespace Proof {
	
	class Proof_API Editore3D : public Layer {
	public:
		Editore3D();
		~Editore3D();
		virtual void OnUpdate(FrameTime DeltaTime) override;
		virtual void OnImGuiDraw(FrameTime DeltaTime) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void CreateMaterialEdtior(class MaterialAsset* material);
		void CreateMeshEditor(class MeshAsset* mesh);
		
		Count<Texture2D>m_CheckeboardTexture;
		static bool IsKeyPressedEditor(KeyBoardKey Key);
		static bool IsKeyClickedEditor(KeyBoardKey Key);
		bool m_ViewPortFocused = false;
		
	private:
		
		struct KeyBoardShowEvent {
			bool ShowAll = false;
			bool ShowOne = false;
			
			bool Clicked = false;
			bool Released = false;
			bool DoubleClicked = false;
			bool Pressed = false;
		};
		struct MouseShowEvent {
			bool ShowAll = false;
			bool ShowOne = false;

			bool Clicked = false;
			bool Released = false;
			bool DoubleClicked = false;
			bool Pressed = false;

			bool Movement = false;
			bool Scroll = false;
		};

		struct WindowShowEvent {
			bool ShowAll = false;
			bool ShowOne = false;

			bool Resize = false;
			bool Minimize = false;
			bool Move = false;
			bool Close = false;

			bool Focus = false;
		};
		struct ControllerEvent {
			bool ShowAll = false;
			bool ShowOne = false;

			bool Clicked = false;
			bool Released = false;
			bool DoubleClicked = false;
			bool Pressed = false;

			bool Joystick = false;
			bool Trigger = false;
		};
		KeyBoardShowEvent m_ShowAllKeyBoardEvents;
		MouseShowEvent m_ShowAllMouseEvents;
		WindowShowEvent m_ShowAllWindowEvents;
		ControllerEvent m_ShowAllControllerEvents;
		virtual void OnEvent(Event& e);
		void OnKeyClicked(KeyClickedEvent& e);
		bool m_ViewPoartHoveredorFocused = false;
		Entity m_CopyEntity;
		void Logger();
		bool m_ShowLogSettings = false;;
		bool m_ClearLogOnPlay = false;
		bool Docking = false;
		bool m_ShowLogger = true;
		bool m_ShowRendererStats = false;
		bool m_ShowWorldEditor= false;
		void ViewPort();
		void MainToolBar();
		glm::vec2 m_ViewportBounds[2];
		Count<Texture2D>m_PlayButtonTexture;
		Count<Texture2D>m_PauseButtonTexture;
		Count<Texture2D>m_SimulateButtonTexture;
		Count<Texture2D>m_StopButtonTexture;
		glm::vec2 _ViewPortSize;
		void SetDocking(bool *p_open);
		glm::vec2 _ViewPortLocation;
		bool ViewPortResize = false;
		Count<class World>ActiveWorld =nullptr;
		Count<class World>m_EditorWorld =nullptr;

		int GuizmoType = (1u << 0)| (1u << 1)| (1u << 2);// imguizmo bit stuff
		class SceneHierachyPanel m_WorldHierachy{this};;
		class ContentBrowserPanel m_CurrentContentBrowserPanel ={this};
		class AssetManagerPanel m_AssetManagerPanel;
		class PerformancePanel m_PerformancePanel;
		friend class Renderer3D;
		friend class Renderer;
		InputPanel m_InputPanel;
		void Save();
		std::unordered_map<UUID,Panel*>m_AllPanels;
		void PlayWorld();
		void SimulateWorld();
		void SetWorldEdit();
		void PauseWorld();
		std::vector<std::string>CubeMapPaths; 
		Count<Shader> m_SkyBoxShader;
		Count<VertexBuffer> m_SkyBoxBuffer;
		Count<VertexArray> m_SkyBoxVertexArray;
		Count<CubeMap> m_CubeMap;
		WorldRenderer m_WorldRenderer;

	};
}