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
		ECS Registry;
		std::string Name = "DefaultWorld";
		template<class T>
		void OnComponentAdded(Entity* Entity,T* component);
		friend class SceneHierachyPanel;
		friend class Entity;
	};
}