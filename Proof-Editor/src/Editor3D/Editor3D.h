#pragma once
#include "Proof/Core/Layer.h"
#include "Proof/Scene/Entity.h"
namespace Proof {
	
	class Proof_API Editore3D : public Layer {
	public:
		Editore3D();
		~Editore3D();
		static Editore3D* Get(){
			return s_Instance;
		}
		virtual void OnUpdate(FrameTime DeltaTime) override;
		virtual void OnImGuiDraw(FrameTime DeltaTime) override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		// returns if assetEditorWasCreated
		bool CreateAssetEditor(AssetID ID);
		static bool IsKeyPressedEditor(KeyBoardKey Key);
		static bool IsKeyClickedEditor(KeyBoardKey Key);
		bool m_ViewPortFocused = false;
		EditorCamera m_EditorCamera = { 200,200 };
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
		Entity m_CopyEntity;
		void Logger();
		void ViewPort();
		void MainToolBar();
		glm::vec2 m_ViewportBounds[2];
		glm::vec2 m_ViewPortSize;
		void SetDocking(bool *p_open);
		glm::vec2 _ViewPortLocation;
		bool m_IsViewPortResize = false;
		Count<class World>m_ActiveWorld =nullptr;
		Count<class World>m_EditorWorld =nullptr;
		std::unordered_map<UUID,Count<class Panel>>m_AllPanels;

		friend class Renderer3D;
		friend class RendererBase;
		void Save();
		void PlayWorld();
		void SimulateWorld();
		void SetWorldEdit();
		void PauseWorld();

		void NewWorld();
		Special<class WorldRenderer> m_WorldRenderer;

		std::unordered_map<Players, Count<class WorldRenderer>> m_MultiplayerRender;

		uint32_t m_PlayersCount = 1;
		static Editore3D* s_Instance;
	};
}