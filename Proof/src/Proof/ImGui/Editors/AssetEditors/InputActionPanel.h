#pragma once
#include "AssetEditorDetail.h"
#include <vector>
namespace Proof
{
	class InputActionEditorPanel :public AssetEditorDetail
	{
	public:
		InputActionEditorPanel();
		virtual void SetAsset(const Count<class Asset>& asset);
	private:
		virtual void RenderDetailSettings();
		virtual Count<class Asset> GetAsset();
		void PropertyWindow();
	private:
		Count<class InputAction> m_Action;
	};

	void EditInputInteractions(std::vector<Count<class InputInteraction>>& interaction);
}