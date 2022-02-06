#include "PhysicsWorld.h"
#include "IntersectData.h"
#include<iostream>
#include "Collider/Collider.h"
namespace ProofPhysicsEngine {
	PhysicsObject& PhysicsWorld::AddObject(const PhysicsObject& object){
		if (object.m_Collider->GetType() == ColliderType::Sphere) {
			SphereCollider* sphereDereived = (SphereCollider *) object.m_Collider;
			m_SphereColliders.emplace_back(SphereCollider(sphereDereived->Center, sphereDereived->Radius));
			m_Objects.emplace_back(object);
			m_Objects.back().m_Collider = &m_SphereColliders.back();
		}
		if (object.m_Collider->GetType() == ColliderType::Cube) {
			CubeCollider* cubeDereived = (CubeCollider*)object.m_Collider;
			m_CubeColliders.emplace_back(CubeCollider(cubeDereived->Center,cubeDereived->Scale, cubeDereived->Rotation));
			m_Objects.emplace_back(object);
			m_Objects.back().m_Collider = &m_CubeColliders.back();
		}
		return m_Objects.back();
	}
	void PhysicsWorld::Simulate(float delta){
		// RIGID BODY
		{
			for (auto& [id,rigidBody]:m_RigidBodies) {
				if (rigidBody.Gravity == true) 
					GravityData.UpdateForce(rigidBody);
				rigidBody.Update(delta);
			}
		}
	}
	void PhysicsWorld::HandleCollisions(){
		for (uint64_t i = 0; i < m_Objects.size(); i++) {
			 //we are cheicnkng if every object
			 //has collided with every other object
			 //we are doing i+1 so we do not check a collison 
			 //multiple tiems
			if (m_Objects[i].GetCollider()->IsTrigger == true)
				continue;
			
			for (uint64_t j = i + 1; j < m_Objects.size(); j++) { 
				if (m_Objects[i].GetCollider()->IsTrigger == true)
					return;
				IntersectData intersectData = m_Objects[i].m_Collider->Intersect(
					*m_Objects[j].m_Collider
				);
				

				if (intersectData.DoesIntersect()) {
					std::cout << "INTERSECT" << std::endl;
					//*m_Objects[i].Veclocity = *m_Objects[i].Veclocity *-1.f;
					//*m_Objects[j].Veclocity = *m_Objects[j].Veclocity *-1.f;
				}
			}
		}
	}
	RigidBody& PhysicsWorld::AddRigidBody(const RigidBody& object) {
		uint64_t Id = Proof::Random::Int<uint64_t>();
		m_RigidBodies.insert({ Id,object });
		return m_RigidBodies.at(Id);
	}
}