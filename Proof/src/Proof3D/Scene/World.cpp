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
#include "Proof/Renderer/Renderer3D.h"
#include "Proof/Renderer/Renderer2D.h"
#include "Proof3D/Scene/Component.h"
#include "Proof/Resources/Asset/MeshAsset.h"
namespace Proof{
	//Model DefaultModel;
	World::World()
	{
		//DefaultModel = {"cube.obj",false};
	}
	void World::OnUpdateEditor(FrameTime DeltaTime) {
		
		glm::mat4 Projection = glm::perspective(glm::radians(45.f),(float)CurrentWindow::GetWindowWidth() / (float)CurrentWindow::GetWindowHeight(),0.1f,100.0f);
		Renderer3D::BeginContext(Projection,EditorCamera);
		for (MeshComponent* Comp : Registry.SceneMeshComponents) {
			if (Comp->m_Asset != nullptr){
				if(Comp->m_Asset->m_Model != nullptr){
					Renderer3D::Draw(*Comp);
				}
			}
		}
		
		Renderer2D::BeginContext(Projection,EditorCamera);
		for (SpriteComponent* Comp : Registry.SpriteComponents) {
			Renderer2D::DrawQuad(*Comp); 
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

	void World::EndRuntime() {
	}

	template<class T>
	void World::OnComponentAdded(Entity _Entity,T* component) {
		PF_CORE_ASSERT(false,"Component Not Identified");
	}
	template<>
	void World::OnComponentAdded(Entity _Entity,NativeScriptComponent* component) {
		static_cast<Component*>(component)->m_EntityOwner = _Entity;
	}
	template<>
	void World::OnComponentAdded(Entity _Entity,TransformComponent* component) {
		static_cast<Component*>(component)->m_EntityOwner = _Entity;
	}
	template<>
	void World::OnComponentAdded(Entity _Entity,TagComponent* component) {
		static_cast<Component*>(component)->m_EntityOwner = _Entity;
	}
	template<>
	void World::OnComponentAdded(Entity _Entity,MeshComponent* component) {
		static_cast<Component*>(component)->m_EntityOwner = _Entity;
	}

	template<>
	void World::OnComponentAdded(Entity _Entity,SpriteComponent* component) {
		static_cast<Component*>(component)->m_EntityOwner = _Entity;
	}
}