#pragma once
#include "Proof/Core/Core.h"

namespace Proof{

	class Proof_API Panel {
	public:
		Panel(bool showWIndow) :
			m_ShowWindow(showWIndow)
		{

		}
		Panel() {

		}
		virtual void ImGuiRender(class FrameTime deltaTime)=0;
		virtual void SetWindowVisibile(bool value){
			m_ShowWindow =value;
		};
	protected:
		bool m_ShowWindow = true;
		friend class Editore3D;
	};
};