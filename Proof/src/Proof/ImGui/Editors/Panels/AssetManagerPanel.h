#pragma once
#include "Panel.h"
namespace Proof {
	class AssetManagerPanel: public Panel
	{
	public:
		AssetManagerPanel();
		virtual void OnImGuiRender(const char* dsiplayName, bool& isOpen)override;
	private:
	};
}
