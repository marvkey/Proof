#include "Proofprch.h"
#include "UIPanel.h"
#include "UIMenu.h"
#include "Proof/Utils/VariableSystem/Variable.h"
namespace Proof {
	
	UIPanel::UIPanel()
	{
		Menu = Count<class UIMenu>::Create();
	}
	UIPanel::UIPanel(const UIPanel& other)
	{
		UIMenu(*other.Menu.Get());
		//m_VariableTable = Count<class VariableRegistry>::CreateFrom(other.m_VariableTable);
	}
}
