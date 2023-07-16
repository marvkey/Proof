#pragma once
#include "Proof/Core/Core.h"

#include <Imgui/Imgui.h>
#include "Proof/Imgui/UI.h"
namespace Proof{

	class Panel : public RefCounted {
	public:
		virtual ~Panel() {};
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