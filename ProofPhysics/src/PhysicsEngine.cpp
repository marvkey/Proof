#include "PhysicsEngine.h"
#include "IntersectData.h"
#include<iostream>
namespace ProofPhysicsEngine {
	void PhysicsEngine::AddObject(const PhysicsObject& object){
		m_Objects.emplace_back(object);
	}
	void PhysicsEngine::Simulate(float delta){
		for (PhysicsObject& physicsObject : m_Objects) {
			physicsObject.Update(delta);
		}
	}
	void PhysicsEngine::HandleCollisions(){
		for (uint64_t i = 0; i < m_Objects.size(); i++) {
			// we are cheicnkng if every object
			// has collided with every other object
			// we are doing i+1 so we do not check a collison 
			// multiple tiems
			for (uint64_t j = i + 1; j < m_Objects.size(); j++) { 
				IntersectData intersectData = m_Objects[i].GetBoudingSphere().IntersectBoundingSphere(
					m_Objects[j].GetBoudingSphere()
				);
				if (intersectData.DoesIntersect()) {
					*m_Objects[i].Veclocity = *m_Objects[i].Veclocity *-1.f;
					*m_Objects[j].Veclocity = *m_Objects[j].Veclocity *-1.f;
				}
			}
		}
	}
}