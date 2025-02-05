#include "Proofprch.h"
#include "UIPanel.h"
#include "UIMenu.h"
#include "Proof/Utils/VariableSystem/Variable.h"
#include "Proof/Utils/ContainerUtils.h"
namespace Proof {
	
	UIPanel::UIPanel()
	{
		Menu = Count<class UIMenu>::Create();
	}
	UIPanel::UIPanel(Count<UIPanel> panel)
	{
		Menu = UIMenu::Copy(panel->Menu);

	}
	
	UIPanelInstance::UIPanelInstance(Count<UIPanel> panel)
	{
		SetPanelInstance(panel);
	}
	UIPanelInstance::UIPanelInstance(Count<UIPanelInstance> panel)
	{
		m_UIPanel = panel->GetUIPanel();
		m_InstanceMenu = UIMenu::Copy(panel->GetInstanceMenu());
	}

	UIPanelInstance::UIPanelInstance()
	{
		m_InstanceMenu = Count<UIMenu>::Create();
	}
	void UIPanelInstance::SetPanelInstance(Count<UIPanel> panel)
	{
		m_UIPanel = panel;
		m_InstanceMenu =  UIMenu::Copy(panel->Menu);
	}

	void UILayer::PopIndex(int index)
	{
		Utils::RemoveAtIndex(m_Panels, index);
	}

}
