#pragma once
#include "Proof/ProofCore.h"

namespace Proof
{
	class VariableRegistrySubPanel : public RefCounted
	{
	public:
		VariableRegistrySubPanel(Count<class VariableRegistry> registry);

		void OnImguiRender();
	private:
		Count<class VariableRegistry> m_Registry;
	private:
		void HierarchyPanel();
		void PropertyPanel();
	};
}