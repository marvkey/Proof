#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Core/Hash.h"

#include <Imgui.h>
class ImGuiWindow;
namespace Proof
{
	
	class EditorWorkspace : public RefCounted
	{
	public:
		virtual ~EditorWorkspace() {};
		virtual void OnUpdate(FrameTime ts) {}
		virtual void OnEvent(class Event& e) {}
		bool IsOpen() const { return m_IsOpen; }
		bool IsFocused() { return m_IsFocused; }
		bool IsHovered() { return m_IsHovered; }
		bool IsFocusedOrHovered() { return IsFocused() || IsHovered(); }
		virtual void OnImGuiRender() {};
		void SetOpen(bool isOpen);
		virtual void SetWorldContext(const Count<class World>& context) {}
		ImGuiWindow* GetImGuiWindow();

	protected:
		EditorWorkspace(const std::string& titleAndID);
		virtual void OnWindowStylePush() {}
		virtual void OnWindowStylePop() {}
		virtual void OnOpen() {}
		virtual void OnClose() {}
		std::string m_TitleAndID;
	private:
		ImVec2 m_CurrentSize;
		bool m_IsFocused = false;
		bool m_IsHovered = false;
		bool m_IsOpen = false;
		ImGuiWindow* m_ImGuiWindow = nullptr;

		virtual ImGuiWindowFlags GetWindowFlags() { return 0; }
	private:
		virtual void Render() final;
		friend class EditorWorkspaceManager;
	};
	struct EditorWorkspaceManagerData
	{
		const char* ID = "";
		const char* Name = "";
		Count<EditorWorkspace> EditorWorkspace = nullptr;
		bool IsOpen = false;
	};
	class EditorWorkspaceManager 
	{
	public:
		EditorWorkspaceManager() = default;
		~EditorWorkspaceManager();
		virtual void OnImGuiRender();
		virtual void OnEvent(class Event& e);
		virtual void OnUpdate(FrameTime ts);
		virtual void SetWorldContext(const Count<class World>& context);

		template<typename TPanel, typename... TArgs>
		Count<TPanel> AddWorkspace(const char* strID, bool isOpenByDefault, TArgs&&... args)
		{
			return AddWorkspace<TPanel>(EditorWorkspaceManagerData{ strID, strID, Count<TPanel>::Create(std::forward<TArgs>(args)...), isOpenByDefault });
		}

		template<typename TPanel>
		Count<TPanel> GetWorkspace(const char* strID)
		{
			static_assert(std::is_base_of<EditorWorkspace, TPanel>::value, "PanelManager::AddPanel requires TPanel to inherit from EditorPanel");

			uint32_t id = Hash::GenerateFNVHash32(strID);

			if (m_EditorWorkspaces.find(id) != m_EditorWorkspaces.end())
				return m_EditorWorkspaces.at(id).EditorWorkspace.As<TPanel>();

			PF_EC_ERROR("PanelManager", "Couldn't find panel with id '{0}'", strID);
			return nullptr;
		}
		const std::unordered_map<uint32_t, EditorWorkspaceManagerData>& GetWorkspaceData() {return m_EditorWorkspaces;}

	private:
		template<typename TPanel>
		Count<TPanel> AddWorkspace(const EditorWorkspaceManagerData& panelData)
		{
			static_assert(std::is_base_of<EditorWorkspace, TPanel>::value, "EditorWorkspaceManager::AddPanel requires TPanel to inherit from EditorPanel");

			auto& panelMap = m_EditorWorkspaces;

			uint32_t id = Hash::GenerateFNVHash32(panelData.ID);
			if (panelMap.find(id) != panelMap.end())
			{
				PF_EC_ERROR("A EditorWorkspace with the id '{0}' has already been added.", panelData.ID);
				return nullptr;
			}

			panelMap[id] = panelData;
			return panelData.EditorWorkspace.As<TPanel>();
		}
	private:
		std::unordered_map<uint32_t, EditorWorkspaceManagerData> m_EditorWorkspaces;
	};
}