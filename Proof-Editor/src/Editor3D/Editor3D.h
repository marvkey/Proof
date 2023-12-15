#pragma once
#include "Proof/Core/Layer.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Camera/EditorCamera.h"
#include "Proof/Input/KeyCodes.h"
#include "Proof/Events/KeyEvent.h"
namespace Proof {
	
	class Editore3D : public Layer 
	{
	public:
		Editore3D();
		~Editore3D();
		static Editore3D* Get(){
			return s_Instance;
		}
		virtual void OnUpdate(FrameTime DeltaTime) override;
		virtual void OnImGuiDraw() override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		// returns if assetEditorWasCreated
		static bool IsKeyPressedEditor(KeyBoardKey Key);
		static bool IsKeyClickedEditor(KeyBoardKey Key);
		bool m_ViewPortFocused = false;
		class EditorCamera m_EditorCamera;
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
		bool OnKeyClicked(KeyClickedEvent& e);
		Entity m_CopyEntity;
		void Logger();
		void ViewPort();
		void MainToolBar();
		glm::vec2 m_ViewportBounds[2];
		glm::vec2 m_ViewPortSize;
		void SetDocking(bool *p_open);
		void SetActiveWorld(Count<World> world);
		glm::vec2 _ViewPortLocation;
		bool m_IsViewPortResize = false;
		Count<class World>m_ActiveWorld =nullptr;
		Count<class World>m_EditorWorld =nullptr;

		friend class Renderer3D;
		friend class RendererBase;
		bool Save();
		void PlayWorld();
		void SimulateWorld();
		void SetWorldEdit();
		void PauseWorld();

		void NewWorld();
		Count<class WorldRenderer> m_WorldRenderer;

		std::unordered_map<Players, Count<class WorldRenderer>> m_MultiplayerRender;

		uint32_t m_PlayersCount = 1;
		static Editore3D* s_Instance;
	private:
		void OpenWorld(AssetID ID);
		void UI_StatisticsPanel();
		void UI_HandleAssetDrop();
		void UI_ShowCreateNewMeshPopup();
		void UI_ShowInvalidAssetMetadataPopup();
		void UI_SaveWorldAs();
	};
}