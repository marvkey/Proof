#pragma once
#include "Panel.h"

namespace Proof
{
	class ScriptSettingsPanelPanel : public Panel
	{
	public:
		void OnImGuiRender(const char* dsiplayName, bool& isOpen)override;
	private:
		bool m_Modified = false;
		std::string m_SearchString;
	};
}