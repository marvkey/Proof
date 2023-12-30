#pragma once
#include "Panel.h"

namespace Proof
{
	class ProjectSettingsPanel : public Panel
	{
	public:
		ProjectSettingsPanel();
		~ProjectSettingsPanel();
		virtual void OnImGuiRender(const char* dsiplayName, bool& isOpen) override;
	private:
		void RenderGeneralSettings();
		void RenderPhysicsSettings();
	private:
		Count<class Project> m_Project;
		//physics
		uint32_t m_PhysicsSelectedLayer = 0;
	};
}