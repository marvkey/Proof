#pragma once
#include "Proof/ProofCore.h"

namespace Proof{
	class SceneHierachyPanel{
	public:
		void OnUpdate(FrameTime DeltaTime);
		void ImGuiOnUpdate();
		void OnAttach();
		void SetContext(World* CurrentWorld){m_CurrentWorld =CurrentWorld;}
	private:
		World* m_CurrentWorld;
		Entity m_SelectedEntity;
		void DrawEntityNode(class Entity& entity);
		void DrawComponent(class Entity& entity);
		static void DrawVectorControl(const std::string& UniqeLabel,Vector& Vec,float ResetValue =0.0f,float columnWidth =100.0f);
	};
}

