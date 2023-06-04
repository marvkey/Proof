#pragma once
#include "Proof/Core/Core.h"
#include "Panel.h"
#include "Proof/Scene/Material.h"
#include "Proof/Renderer/ParticleSystem.h"
#include "Proof/Scene/Camera/EditorCamera.h"
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

	class Proof_API ParticleSystemPanel : public Panel {
	public:
		ParticleSystemPanel(Count<ParticleSystem> particle);
		virtual void ImGuiRender(FrameTime deltaTime)override;
	private:
		float m_Save = 10.0f;
		Vector2 m_Size;
		Count<ParticleHandler> m_ParticleHandler;
		Count<ParticleSystem> m_ParticleSystem;
		Count<class  WorldRenderer> m_WorldRenderer;
		Count<class World> m_World;
		EditorCamera m_Camera{ 200,200 };
	};
}
