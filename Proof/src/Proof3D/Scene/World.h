#pragma once
#include "Proof3D/EntitiyComponentSystem/ECS.h"
#include "Proof3D/Renderer/Camera/EditorCamera.h"
#include "Proof3D/Renderer/Camera/OrthagraphicCamera.h"
class FrameTime;

namespace Proof{
	class World {
	public:
		World();
		virtual void OnUpdateEditor(FrameTime DeltaTime);
		virtual void OnUpdateRuntime(FrameTime DeltaTime);
		class Entity CreateEntity(const std::string& EntName);
		virtual void EndRuntime();
		std::string GetName(){return Name;};
		class ECS Registry;
	private:
		std::string Name = "DefaultWorld";
		template<class T>
		void OnComponentAdded(Entity Entity,T* component);
		friend class SceneHierachyPanel;
		friend class Entity;
		friend class WorldScerelizer;
		friend class Editor3D;
		class EditorCamera3D EditorCamera;

		class OrthagraphicCamera SceneCamera { -1.0f,1.0f,-1.0f,1.0f };
	};
}