#pragma once
#include "entt/entt.hpp"
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
		entt::registry Registry;
		friend class Entity;
		template<class T>
		void OnComponentAdded(Entity* Entity,T& Component);
		friend class SceneHierachyPanel;
	};
}