#pragma once
#include "Panel.h"
#include "Proof/Scene/Entity.h"
namespace Proof
{
	class SceneHierachyPanel: public Panel
	{
	public:
		SceneHierachyPanel(bool IsWorld =true, UUID prefabID = 0);
		void OnImGuiRender(const char* dsiplayName, bool& isOpen);
		static void DrawVectorControl(const std::string& UniqeLabel, Vector& Vec, float ResetValue = 0.0f, float columnWidth = 100.0f, float Speed = 0.1f);
		static void DrawVectorControl(const std::string& UniqeLabel, glm::vec3& Vec,float ResetValue =0.0f,float columnWidth =100.0f,float Speed=0.1f);
		static void DrawVector4Control(const std::string& UniqeLabel, glm::vec4& Vec, float ResetValue = 0.0f, float columnWidth = 100.0f);
		static void DrawVector2Control(const std::string& UniqeLabel,glm::vec2& Vec,float ResetValue = 0.0f,float columnWidth = 100.0f);
		static void DrawVectorControl(const std::string& UniqeLabel, VectorTemplate<bool>& Vec, bool resetValue = true,float columnWidth = 100.0f);
		//Entity GetSelectedEntity() { return m_SelectedEntity; };
		//void SetSelectedEntity(Entity entity){m_SelectedEntity = entity;};
		virtual void SetWorldContext(const Count<class World>& world) { m_ActiveWorld = world; }
		bool IsFocused() { return m_IsFocused; }
		bool IsHovered() { return m_IsHovered; }

		bool IsHoveredOrFocused() { return IsFocused() || IsHovered(); }
	private:
		bool m_IsWorld = true;
		UUID m_PrefabID = 0;
		bool m_IsHovered = false;
		bool m_IsFocused = false;
		bool m_WindowHoveredorFocus = false;
		bool CreateEntityMenu(Entity owner = {});
		Count<class World> m_ActiveWorld = nullptr;
		//Entity m_SelectedEntity = {};
		void DrawEntityNode(class Entity entity); 
		void DrawComponent(class Entity& entity);

		template<typename T,typename UIFunction>
		void DrawComponents(const std::string&name, Entity& entity,UIFunction Uifunction,const std::string& toolTip=std::string());
		
	};
}

