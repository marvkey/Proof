#pragma once
#include "Panel.h"
namespace Proof {
	class AssetManagerPanel: public Panel
	{
	public:
		virtual void ImGuiRender(FrameTime deltaTime)override;
		AssetManagerPanel() { m_ShowWindow = false; }
	};
}
