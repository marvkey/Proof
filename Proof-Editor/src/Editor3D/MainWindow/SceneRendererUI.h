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
	
		void RenderAsset(FrameTime deltaTime);
	private:
		Entity tempEntity;
		void MeshUI();
		uint32_t m_LastWidht=0,m_LastHeight=0;
		UUID m_ID = UUID();
		class MeshComponent* mesh =  nullptr;
		Count<World> m_World = nullptr;
		class MeshAsset* m_MeshAsset = nullptr;
		WorldRenderer m_WorldRenderer;
		SceneRendererType m_Type;
	};
}