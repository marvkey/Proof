#pragma once
#include "Proof/Core/Core.h"
#include "Proof/COre/FrameTime.h"
#include "Proof/Asset/AssetTypes.h"
#include "Proof/Asset/Asset.h"
#include <imgui.h>
namespace Proof
{
	
	class AssetEditor : public RefCounted
	{
	protected:
		AssetEditor(const char* id);
	public:
		virtual ~AssetEditor() {}

		virtual void OnUpdate(FrameTime ts);
		virtual void OnEvent(class Event& e);
		// of the editore 3d
		virtual void SetWorldContext(const Count<class World>& context) {}
		virtual void OnImGuiRender() {};
		void SetOpen(bool isOpen);
		void SetTitle(struct AssetInfo assetInfo);
		virtual void SetAsset(const Count<class Asset>& asset) = 0;

		bool IsOpen() const { return m_IsOpen; }
		//if the current window is focused
		bool IsFocused() { return m_IsFocused; }
		bool IsHovered() { return m_IsHovered; }

		bool IsHoveredOrFocused() { return IsFocused() || IsHovered(); }
		ImVec2 GetWindowSize(){ return m_CurrentSize;}

		virtual bool IsSaved() = 0;
		virtual void Save() = 0;
	protected:
		static inline float SavedPresetSeconds = 300.f; // 5 minutes per save

		void SetMinSize(uint32_t width, uint32_t height);
		void SetMaxSize(uint32_t width, uint32_t height);

		virtual ImGuiWindowFlags GetWindowFlags() { return 0; }

		// Subclass can optionally override these to customize window styling, e.g. window padding
		virtual void OnWindowStylePush() {}
		virtual void OnWindowStylePop() {}
		virtual void OnOpen() {}
		virtual void OnClose() {}
	protected:
		std::string m_TitleAndId;  // Caches title###asetType+id to avoid computing it every frame
		ImVec2 m_MinSize, m_MaxSize;

		bool m_IsOpen = false;
	private:
		virtual void Render()final;
	private:
		float m_SaveCountDown = SavedPresetSeconds;

		ImVec2 m_CurrentSize;
		bool m_IsFocused = false;
		bool m_IsHovered = false;

		friend class AssetEditorPanel;
		UUID32 m_PushID = UUID32();
		std::string m_Id;          // Uniquely identifies the window independently of its title (e.g. for imgui.ini settings
	};

	class AssetEditorPanel
	{
	public:
	
		static void OnUpdate(FrameTime ts);
		static void OnEvent(Event& e);
		static void SetSceneContext(const Count<World>& context);
		static void OnImGuiRender();
		static void OpenEditor(const Count<Asset>& asset);

		static void RegisterEditor(AssetType type)
		{
			PF_CORE_ASSERT(s_Editors.find(type) == s_Editors.end(), "There's already an editor for that asset!");
			s_Editors[type] = {};
		}

	private:
		static void RegisterDefaultEditors();
		static void UnregisterAllEditors();

		// asset Type, asset id, and the editor
		static std::unordered_map<AssetType, std::unordered_map<UUID, Count<AssetEditor>>> s_Editors;
		static Count<World> s_SceneContext;

		friend class Editore3D;
	};
}