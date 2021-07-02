#pragma once
#include "entt/entt.hpp"
#include "Proof3D/EntitiyComponentSystem/ECS.h"
class FrameTime;

namespace Proof
{
	class World {
	public:
		virtual void OnUpdateEditor(FrameTime DeltaTime);
		virtual void OnUpdateRuntime(FrameTime DeltaTime);
		class Entity CreateEntity(const std::string& EntName);
		virtual void EndRuntime();
	private:
		std::string Name = "DefaultWorld";
		//entt::registry Registry;
		ECS Registry;
		template<class T>
		void OnComponentAdded(Entity* Entity,T* component);
		friend class SceneHierachyPanel;
		friend class Entity;
	};
}