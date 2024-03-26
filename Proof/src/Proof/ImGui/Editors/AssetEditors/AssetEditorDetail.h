#pragma once
#include "AssetEditor.h"
namespace Proof
{
	class AssetEditorDetail : public AssetEditor
	{
	public:
		AssetEditorDetail(const std::string& windowTitle);
	protected:
		virtual void RenderDetailSettings() = 0;
		virtual void SetAsset(const Count<class Asset>& asset);
		virtual Count<class Asset> GetAsset() = 0;
		virtual void OnImGuiRender() override final;
		virtual bool IsSaved() { return !m_NeedsSaving; };
		virtual void Save();
		virtual void SetDefaultLayout();
		virtual bool IsSubWindowsHovered();
		virtual bool IsSubWindowsFocused();
	protected:
		bool m_NeedsSaving = true;
		Count< class DetailsPanel> m_DetailsPanel;
		bool m_IsDetailsPanelOpen = true;
		std::string m_DetailsPanelName;
	};
}