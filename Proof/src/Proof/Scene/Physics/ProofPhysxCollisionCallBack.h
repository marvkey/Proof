#pragma once
#include <PxPhysicsAPI.h>

namespace Proof{
	class ProofPhysxCollisionCallback :public physx::PxSimulationEventCallback {
		physx::PxActor* mSubmarineActor;
		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) {};
		virtual void onWake(physx::PxActor** actors, physx::PxU32 count) {};
		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) {};
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) {};

		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
	};
}