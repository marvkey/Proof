#pragma once
#include "Proof/ProofCore.h"

namespace Proof{
	class SceneHierachyPanel{
	public:
		void ImGuiOnUpdate();
		void SetContext(World* CurrentWorld){m_CurrentWorld =CurrentWorld;}
		static void DrawVectorControl(const std::string& UniqeLabel,Vector& Vec,float ResetValue =0.0f,float columnWidth =100.0f);
		static void DrawVector4Control(const std::string& UniqeLabel,glm::vec4& Vec,float ResetValue = 0.0f,float columnWidth = 100.0f);
		Entity GetSelectedEntity(){return m_SelectedEntity;};
	private:
		World* m_CurrentWorld;
		Entity m_SelectedEntity;
		void DrawEntityNode(class Entity& entity);
		void DrawComponent(class Entity& entity);

		template<typename T,typename UIFunction>
		static void DrawComponents(const std::string&name, Entity& entity, T* Comp,uint32_t IndexValue,UIFunction Uifunction);
	};
}

