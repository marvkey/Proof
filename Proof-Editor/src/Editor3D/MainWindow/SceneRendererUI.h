#pragma once
#include "Proof/ProofCore.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Resources/Asset/MeshAsset.h"
#include "Proof/Scene/World.h"
#include "Panel.h"

namespace Proof
{
	enum class SceneRendererType{
		MeshAsset,World,EntityPrefab
	};
	class Proof_API SceneRendererUI:public Panel{
		virtual void ImGuiRender(FrameTime deltaTime)override;

	public:
		SceneRendererUI(MeshAsset* asset);
		virtual void SetWindowVisibile(bool value){
			m_ShowWindow =value;
		}
		void RenderAsset(FrameTime deltaTime);
	private:
		Entity tempEntity;
		void MeshUI();
		bool m_ShowWindow = true;
		uint32_t m_LastWidht=0,m_LastHeight=0;
		uint64_t m_ID = Math::RandUINT<uint64_t>(1,18000000000000000000);
		class MeshComponent* mesh;
		Special<World> m_World;
		class MeshAsset* m_MeshAsset;
		WorldRenderer m_WorldRenderer;
		SceneRendererType m_Type;
	};
}