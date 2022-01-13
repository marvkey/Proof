#include "Proofprch.h"
#include "PhysicsEngine.h"
#include "Component.h"
#include "World.h"
#include "Entity.h"
namespace Proof {
	void PhysicsEngine::Start(){

		auto& spherColliderView = m_World->m_Registry.view<SphereColliderComponent>();
		for (auto entity : spherColliderView) {
			auto& sphereCollider = spherColliderView.get<SphereColliderComponent>(entity);
			auto& collider = m_PhysicsEngine.AddObject(ProofPhysicsEngine::PhysicsObject(ProofPhysicsEngine::SphereCollider(Entity{ entity,m_World }.GetComponent<TransformComponent>()->Location + sphereCollider.Offset, sphereCollider.Radius)));
			sphereCollider.RuntimeBody = collider.GetCollider();
		}

		auto& cubeColliderView = m_World->m_Registry.view<CubeColliderComponent>();
		for (auto entity : cubeColliderView) {
			auto& cubeCollider = cubeColliderView.get<CubeColliderComponent>(entity);
			const auto& transform = Entity{entity,m_World }.GetComponent<TransformComponent>();

			auto& collider = m_PhysicsEngine.AddObject(ProofPhysicsEngine::PhysicsObject(ProofPhysicsEngine::CubeCollider(transform->Location + cubeCollider.OffsetLocation, transform->Rotation, transform->Scale + cubeCollider.OffsetLocation)));
			cubeCollider.RuntimeBody = collider.GetCollider();
		}
	}
	void PhysicsEngine::Update(float delta)
	{
		
		// Sphere Collider
		{
			auto& sccV = m_World->m_Registry.view<SphereColliderComponent>();
			for (auto entity : sccV) {
				Entity currentEntity{ entity, m_World};
				auto& sphereCollider = sccV.get<SphereColliderComponent>(entity);
				auto* collider = (ProofPhysicsEngine::SphereCollider*)sphereCollider.RuntimeBody;
				collider->Center = sphereCollider.Offset + currentEntity.GetComponent<TransformComponent>()->Location;
				collider->Radius = sphereCollider.Radius * currentEntity.GetComponent<TransformComponent>()->Scale.GetMax();
			}
		}
		// Cube Collider
		{
			auto& cccV = m_World->m_Registry.view<CubeColliderComponent>();
			for (auto entity : cccV) {
				Entity currentEntity{ entity, m_World};
				const auto& transform = *currentEntity.GetComponent<TransformComponent>();
				auto& cubeCollider = cccV.get<CubeColliderComponent>(entity);
				auto* collider = (ProofPhysicsEngine::CubeCollider*)cubeCollider.RuntimeBody;
				collider->Center = transform.Location + cubeCollider.OffsetLocation;
				collider->Rotation = transform.Rotation;
				collider->Scale = transform.Scale + cubeCollider.OffsetScale;
			}
		}
		m_PhysicsEngine.Simulate(delta);
		m_PhysicsEngine.HandleCollisions();
	}
	void PhysicsEngine::End(){
		
	}
}