#pragma once
#include "Proof/ProofCore.h"
#include "Panel.h"
namespace Proof{
	class SceneHierachyPanel: public Panel{
	public:
		SceneHierachyPanel(class Editore3D* owner) :
			m_EditorOwner(owner) {};
		void ImGuiRender(class FrameTime deltaTime);
		void SetContext(World* CurrentWorld){m_CurrentWorld =CurrentWorld;}
		static void DrawVectorControl(const std::string& UniqeLabel,Vector& Vec,float ResetValue =0.0f,float columnWidth =100.0f,float Speed=0.1f);
		static void DrawVector4Control(const std::string& UniqeLabel,glm::vec4& Vec,float ResetValue = 0.0f,float columnWidth = 100.0f);

		static void DrawVectorControl(const std::string& UniqeLabel, VectorTemplate<bool>& Vec, bool resetValue = true,float columnWidth = 100.0f);
		Entity GetSelectedEntity(){return m_SelectedEntity;};

		void SetBrowserPanel(class ContentBrowserPanel* Panel){
			//m_BrowserPanel =  Panel;
		}
		bool m_WindowHoveredorFocus = false;
		void OnKeyClicked(KeyClickedEvent& e);

	private:
		template<class T>
		void AddComponentGui(Entity entity,const std::string& name) {
			if (ImGui::MenuItem(name.c_str())) {
				entity.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
		Editore3D* m_EditorOwner = nullptr;
		// Retuns True of an entity is created
		bool CreateEntityMenu(Entity owner = {});
		World* m_CurrentWorld = nullptr;
		Entity m_SelectedEntity = {};
		//class ContentBrowserPanel* m_BrowserPanel =nullptr;
		void DrawEntityNode(class Entity entity); 
		void DrawComponent(class Entity& entity);

		template<typename T,typename UIFunction>
		void DrawComponents(const std::string&name, Entity& entity,UIFunction Uifunction,const std::string& toolTip=std::string());
		
		friend class Editore3D;
	};
}

