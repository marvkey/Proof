#pragma once
#include "Proof/Core/Core.h"
#include "../Panel.h"
namespace Proof
{

	class Proof_API RenderPanel:public Panel {
	public:
		virtual void ImGuiRender(class FrameTime deltaTime);
		virtual void SetRenderData(class RendererData& rendererData,const std::string& worldName){
			m_RenderData = &rendererData;
			m_WorldName = worldName;
		}
	private:
		struct RendererData* m_RenderData;
		void RenderDefered();
		void RenderFoward();
		std::string m_WorldName;
	};
};