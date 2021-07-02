#include "Proofprch.h"
#include "World.h"
#include "Component.h"
#include "Entity.h"
#include "ScriptableEntity.h"
#include "Proof/Renderer/Renderer.h"
#include "Model.h"
#include "Mesh.h"
#include "Proof/Core/FrameTime.h"
#include "Component.h"
#include <iostream>
namespace Proof{
	void World::OnUpdateEditor(FrameTime DeltaTime) {}

	void World::OnUpdateRuntime(FrameTime DeltaTime) {
		Registry.view<NativeScriptComponent>().each([=](auto _Entity,auto& Nsc) {
			if (!Nsc.Instance) {
				Nsc.Instance = Nsc.InstantiateScript();
				Nsc.Instance->OwnerEntity = Nsc.EntityOwner;
				Nsc.Instance->OnCreate();
				Nsc.Instance->OnlyOnCreate();
			}
			Nsc.Instance->OnUpdate(DeltaTime);
		});
	}

	Entity World::CreateEntity(const std::string& EntName) {
		Entity entity = {Registry.create(),this};
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>()->SetName(EntName);
		return entity;
	}

	void World::EndRuntime() {
		Registry.view<NativeScriptComponent>().each([=](auto _Entity,auto& Nsc) {
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
	void World::OnComponentAdded(Entity* _Entity,T& Component) {

	}
	template<>
	void World::OnComponentAdded(Entity* _Entity,NativeScriptComponent& Component) {
		Component.EntityOwner = _Entity;
	}
	template<>
	void World::OnComponentAdded(Entity* _Entity,TransformComponent& Component) {
	

	}
	template<>
	void World::OnComponentAdded(Entity* _Entity,TagComponent& Component) {
		Component.m_EntityOwner = _Entity;
	}
	template<>
	void World::OnComponentAdded(Entity* _Entity,MeshComponent& Component) {
		Component.m_EntityOwner = _Entity;
	}
}