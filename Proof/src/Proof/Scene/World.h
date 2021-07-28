#pragma once
#include "EntitiyComponentSystem/ECS.h"
#include "Camera/EditorCamera.h"
#include "Camera/OrthagraphicCamera.h"
class FrameTime;

namespace Proof{
	class World {
	public:
		World();
		virtual void OnUpdateEditor(FrameTime DeltaTime);
		virtual void OnUpdateRuntime(FrameTime DeltaTime);
		class Entity CreateEntity(const std::string& EntName);
		class Entity CreateEntity(const std::string& EntName,uint32_t ID);

		virtual void EndRuntime();
		std::string GetName(){return Name;};
		class ECS Registry;
		std::string GetPath(){
			return m_Path;
		}
	private:
		std::string Name = "DefaultWorld";
		template<class T>
		void OnComponentAdded(Entity Entity,T* component);
		friend class SceneHierachyPanel;
		friend class Entity;
		friend class Editor3D;
		class EditorCamera3D EditorCamera;
		class OrthagraphicCamera SceneCamera { -1.0f,1.0f,-1.0f,1.0f };
		friend class SceneSerializer;
		friend class Component;
		friend class Editore3D;
		std::string m_Path;
		glm::mat4 Projection;
	};
}