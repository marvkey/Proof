#pragma once
#include "Panel.h"

namespace Proof
{
	class WorldRendererPanel : public Panel
	{
	public:
		WorldRendererPanel();
		void SetContext(const Count<class WorldRenderer>& context) { m_WorldRenderer = context; }

		void OnImGuiRender(const char* dsiplayName, bool& isOpen)override;
		Count<class WorldRenderer> GetWorldRenderer()
		{
			return m_WorldRenderer;
		};

	protected:
		Count<class WorldRenderer> m_WorldRenderer;
	};

}