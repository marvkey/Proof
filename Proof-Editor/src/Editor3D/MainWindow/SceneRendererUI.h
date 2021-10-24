#pragma once
#include "Proof/ProofCore.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Resources/Asset/MeshAsset.h"

#include "Panel.h"

namespace Proof
{
	enum class SceneRendererType{
		MeshAsset=0
	};
	class Proof_API SceneRendererUI:public Panel{
		virtual void ImGuiRender(FrameTime deltaTime)override;

	public:
		SceneRendererUI(MeshAsset* asset);
		virtual void SetWindowVisibile(bool value){
			m_ShowWindow =value;
		}

	private:
		Entity tempEntity;
		void UI();
		bool m_ShowWindow = true;
		uint32_t m_LastWidht=0,m_LastHeight=0;
		uint64_t m_ID = Math::RandUINT(1,18000000000000000000);
		class MeshComponent* mesh;
		class World* m_World;
		WorldRenderer m_WorldRenderer;
		SceneRendererType m_Type = SceneRendererType::MeshAsset;
	};
}