#pragma once
#include "Proof/Core/Core.h"
#include "Panel.h"

namespace Proof
{
	class Material;
	class PhysicsMaterial;
	class Proof_API MaterialEditorPanel:public Panel {
	public:
		MaterialEditorPanel(Count<Material> material );
		virtual void ImGuiRender(FrameTime deltaTime)override;
	
	private:
		Count<Material> m_Material;
		//Count<Texture2D>m_CheckeboardTexture;
	};
	class Proof_API PhysicsMaterialEditorPanel :public Panel {
	public:
		PhysicsMaterialEditorPanel(Count<PhysicsMaterial> material);
		virtual void ImGuiRender(FrameTime deltaTime)override;

	private:
		Count<PhysicsMaterial> m_Material;

	};
}
