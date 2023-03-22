#include"RenderPanel.h"
#include <Imgui/imgui.h>
#include <Imgui/imgui_internal.h>
#include "../../ImGUIAPI.h"
#include "Proof/Utils/PlatformUtils.h"
#include "../../Editor3D.h"
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"

namespace Proof{
	void RenderPanel::ImGuiRender(FrameTime deltaTime) {
		#if  0
		if (m_ShowWindow == false)
			return;
		if(ImGui::Begin(m_WorldName.c_str(),&m_ShowWindow)){
			if(m_RenderData->RenderSettings.Technique == RenderTechnique::DeferedRendering)
				RenderDefered();
			else if(m_RenderData->RenderSettings.Technique== RenderTechnique::FowardRendering)
				RenderFoward();
		};
		ImGui::End();
		#endif
	}
	void RenderPanel::RenderDefered() {
		
	}
	void RenderPanel::RenderFoward() {
		
	}
}