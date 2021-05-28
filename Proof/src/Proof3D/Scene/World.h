#pragma once
#include "entt/entt.hpp"
class World {
public:
	virtual void OnUpdateEditor(FrameTime DeltaTime);
	virtual void OnUpdateRuntime(FrameTime DeltaTime);
	class Entity CreateEntity();
	virtual void EndRuntime(); 
private:
	std::vector<Entity*>AllEntity;
	std::string Name = "DefaultWorld";
	entt::registry Registry;
	friend class Entity;
	template<class T>
	void OnComponentAdded(Entity* Entity,T&Component);
};