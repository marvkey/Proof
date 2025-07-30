#pragma once
#include "AssetEditor.h"
#include "AssetEditorDetail.h"

namespace Proof
{
	class AnimationControllerPanel :public AssetEditorDetail
	{
	public:
		AnimationControllerPanel();
		virtual void RenderDetailSettings();
	};
}