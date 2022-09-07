#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Asset/MaterialAsset.h"
#include "Proof/Asset/PhysicsMaterialAsset.h"
#include "Panel.h"

namespace Proof
{
	class Proof_API MaterialEditorPanel:public Panel {
	public:
		MaterialEditorPanel(MaterialAsset* material);
		virtual void ImGuiRender(FrameTime deltaTime)override;
	
	private:
		MaterialAsset* m_MaterialAsset;
		uint32_t whiteColourId;
		Count<Texture2D>m_CheckeboardTexture;
	};
	class Proof_API PhysicsMaterialEditorPanel :public Panel {
	public:
		PhysicsMaterialEditorPanel(PhysicsMaterialAsset* material);
		virtual void ImGuiRender(FrameTime deltaTime)override;

	private:
		PhysicsMaterialAsset* m_MaterialAsset;
	};
}
