#include "Proofprch.h"
#include "UIPanel.h"
#include "UIMenu.h"
#include "Proof/Utils/VariableSystem/Variable.h"
#include "Proof/Utils/ContainerUtils.h"
namespace Proof {
	
	UIPanel::UIPanel()
	{
		Menu = Count<class UIMenu>::Create();
		VariableTable = Count<VariableRegistry>::Create();
	}
	UIPanel::UIPanel(Count<UIPanel> panel)
	{
		Menu = UIMenu::Copy(panel->Menu);
		VariableTable = Count<VariableRegistry>::Create();
	}
	
	UIPanelInstance::UIPanelInstance(Count<UIPanel> panel)
	{
		SetPanelInstance(panel);
	}

	UIPanelInstance::UIPanelInstance(Count<UIPanelInstance> panel)
	{
		m_UIPanel = panel->GetUIPanel();
		m_InstanceMenu = UIMenu::Copy(panel->GetInstanceMenu());
		m_VariableRegistryInstance = Count<VariableRegistryInstance>::Create(panel->m_VariableRegistryInstance);

		m_InstanceMenu->SetVariableStorageSet(m_VariableRegistryInstance->GetVariabelSetStorage());
	}

	UIPanelInstance::UIPanelInstance()
	{
		m_InstanceMenu = Count<UIMenu>::Create();
	}

	void UIPanelInstance::SyncWithPanel()
	{
		if (m_UIPanel)
		{
			m_InstanceMenu = UIMenu::Copy(m_UIPanel->Menu);
			if (m_VariableRegistryInstance == nullptr || m_VariableRegistryInstance->GetVariableRegistry() != m_UIPanel->VariableTable)
				m_VariableRegistryInstance = Count<VariableRegistryInstance>::Create(m_UIPanel->VariableTable);
			else
				m_VariableRegistryInstance->SyncWithRegistry();
		}

	}
	void UIPanelInstance::SetPanelInstance(Count<UIPanel> panel)
	{
		m_UIPanel = panel;
		SyncWithPanel();
	}

	void UILayer::PopIndex(int index)
	{
		Utils::RemoveAtIndex(m_Panels, index);
	}

}
