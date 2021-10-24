#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Resources/Asset/MaterialAsset.h"
#include "Panel.h"

namespace Proof
{
	class Proof_API MaterialEditorPanel:public Panel {
	public:
		MaterialEditorPanel(MaterialAsset* material);
		virtual void ImGuiRender(FrameTime deltaTime)override;
		virtual void SetWindowVisibile(bool value) {
			m_ShowWindow = value;
		}
	private:
		bool m_ShowWindow =true;
		MaterialAsset* m_MaterialAsset;
		uint32_t whiteColourId;
		Count<Texture2D>m_CheckeboardTexture;
	};
}
