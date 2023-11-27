#pragma once
#include "Panel.h"
namespace Proof
{
	class PhysicsStatsPanel : public Panel
	{
	public:
		PhysicsStatsPanel();
		~PhysicsStatsPanel();

		virtual void SetWorldContext(const Count<class World>& context) override;
		virtual void OnImGuiRender(const char* dsiplayName, bool& isOpen) ;

	private:
		Count<class PhysicsWorld> m_PhysicsWorld;
	};
}