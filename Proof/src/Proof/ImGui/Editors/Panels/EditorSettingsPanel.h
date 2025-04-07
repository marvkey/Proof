#pragma once
#include "Panel.h"

namespace Proof
{
	class EditorSettingsPanel : public Panel
	{
	public:
		EditorSettingsPanel();
		~EditorSettingsPanel();
		virtual void OnImGuiRender(const char* dsiplayName, bool& isOpen) override;
	private:
	};
}