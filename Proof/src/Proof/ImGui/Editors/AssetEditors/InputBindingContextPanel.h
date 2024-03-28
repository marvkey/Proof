#pragma once
#include "AssetEditorDetail.h"
#include "Proof/Input/ElevatedInputSystem/InputKeyBinding.h"
#include <vector>
namespace Proof
{
	class InputBindingContextPanel : public AssetEditorDetail
	{
	public:
		InputBindingContextPanel();
		virtual void SetAsset(const Count<class Asset>& asset);
		struct SelectedInputBinding
		{
			WeakCount<class InputKeyBindingBase> SelectedInputKeyBinding;
			bool InputBindingBundle = false;
		};
	private:
		virtual void RenderDetailSettings();
		virtual Count<class Asset> GetAsset();
		void PropertyWindow();
	private:
		Count<class InputBindingContext> m_BindingContext;
		SelectedInputBinding m_SelectedInputBinding;
	};
}