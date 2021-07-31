#include "Proofprch.h"
#include "World.h"
#include "Component.h"
#include "Entity.h"
#include "Script.h"
#include "Proof/Renderer/Renderer.h"
#include "Model.h"
#include "Mesh.h"
#include "Proof/Core/FrameTime.h"
#include "Component.h"
#include "Proof/Renderer/Renderer3D.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof/Scene/Component.h"
#include "Proof/Resources/Asset/MeshAsset.h"

namespace Proof{
	World::World()
	{

	}
	void World::OnUpdateEditor(FrameTime DeltaTime) {
		
		Projection = glm::perspective(glm::radians(45.f),(float)CurrentWindow::GetWindowWidth() / (float)CurrentWindow::GetWindowHeight(),0.1f,100.0f);
		Renderer2D::BeginContext(Projection,EditorCamera.GetCameraView());
		for (SpriteComponent* Comp : Registry.SpriteComponents) {
			Renderer2D::DrawQuad(*Comp);
		}

		Renderer3D::BeginContext(Projection,EditorCamera);
		for (MeshComponent* Comp : Registry.SceneMeshComponents) {
			if (Comp->GetModel()!= nullptr){
				Renderer3D::Draw(*Comp);
			}
		}

		for (NativeScriptComponent* Scripts : Registry.NativeScripts) {
			if(Scripts->m_HasbeenInstanciated == false)
				continue;
			if (Scripts->Instance) {
				Scripts->Instance = Scripts->InstantiateScript();
				Scripts->Instance->OwnerEntity = Scripts->GetOwner();
				Scripts->Instance->OnCreate();
				Scripts->Instance->OnlyOnCreate();
			}
			Scripts->Instance->OnUpdate(DeltaTime);
		}

		
		EditorCamera.OnUpdate(DeltaTime);
	}

	void World::OnUpdateRuntime(FrameTime DeltaTime) {
		/*
		Registry.view<NativeScriptComponent>().each([=](auto _Entity,auto& Nsc) {
			if (!Nsc.Instance) {
				Nsc.Instance = Nsc.InstantiateScript();
				Nsc.Instance->OwnerEntity = Nsc.GetOwner();
				Nsc.Instance->OnCreate();
				Nsc.Instance->OnlyOnCreate();
			}
			Nsc.Instance->OnUpdate(DeltaTime);
		});
		*/

	}

	Entity World::CreateEntity(const std::string& EntName) {
		Entity entity = {Registry.Create(),this};
		entity.AddComponent<TagComponent>()->Name =EntName;
		entity.AddComponent<TransformComponent>();

		return entity;
	}

	Entity World::CreateEntity(const std::string& EntName,uint32_t ID) {
		Entity entity = {Registry.Create(ID),this};
		entity.AddComponent<TagComponent>()->Name = EntName;
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void World::EndRuntime() {
	}

	template<class T>
	void World::OnComponentAdded(Entity _Entity,T* component) {
		PF_CORE_ASSERT(false,"Component Not Identified");
	}
	template<>
	void World::OnComponentAdded(Entity _Entity,NativeScriptComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;
	}
	template<>
	void World::OnComponentAdded(Entity _Entity,TransformComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;

	}
	template<>
	void World::OnComponentAdded(Entity _Entity,TagComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;

	}
	template<>
	void World::OnComponentAdded(Entity _Entity,MeshComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;
	}

	template<>
	void World::OnComponentAdded(Entity _Entity,SpriteComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;
	}

	template<>
	void World::OnComponentAdded(Entity _Entity,LightComponent* component) {
		Component* a = static_cast<Component*>(component);
		a->m_EntityOwner = _Entity.GetID();
		a->CurrentWorld = this;
	}
}