#include "Proofprch.h"
#include "World.h"
#include "Component.h"
#include "Entity.h"
#include "ScriptableEntity.h"
void World::OnUpdateEditor(FrameTime DeltaTime) {}

void World::OnUpdateRuntime(FrameTime DeltaTime) {
	Registry.view<NativeScriptComponent>().each([=](auto _Entity,auto&Nsc) {
		if (!Nsc.Instance) {
			Nsc.Instance = Nsc.InstantiateScript();
			Nsc.Instance->OwnerEntity = Nsc.EntityOwner;
			Nsc.Instance->OnCreate();
			Nsc.Instance->OnlyOnCreate();
		}
		Nsc.Instance->OnUpdate(DeltaTime);
	});
}

Entity World::CreateEntity() {
	Entity entity = {Registry.create(),this};
	entity.AddComponent<TransformComponent>();
	entity.AddComponent<TagComponent>();
	return entity;
}

void World::EndRuntime() {
	Registry.view<NativeScriptComponent>().each([=](auto _Entity,auto&Nsc) {
		if (!Nsc.Instance) {
			Nsc.Instance = Nsc.InstantiateScript();
			Nsc.Instance->OwnerEntity = Nsc.EntityOwner;
			Nsc.Instance->OnCreate();
			Nsc.Instance->OnlyOnCreate();
		}
		Nsc.Instance->OnDestroy();
	});
}

template<class T>
void World::OnComponentAdded(Entity* _Entity,T&Component) {

}
template<>
void World::OnComponentAdded(Entity* _Entity,NativeScriptComponent&Component) {
	Component.EntityOwner = _Entity;
}
template<>
void World::OnComponentAdded(Entity* _Entity,TransformComponent&Component) {
}
template<>
void World::OnComponentAdded(Entity* _Entity,TagComponent&Component) {

}
