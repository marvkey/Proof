#pragma once
#include <Physx/PxPhysicsAPI.h>
#include <unordered_map>
namespace Proof
{
	struct PhysicsCollision
	{

	};

	struct PhysicsSimulationTriggerObject
	{
		Count<class PhysicsActorBase> TriggerActor;
		Count<class PhysicsActorBase> OverlapTrigger;
		bool ReadyToCallPersist = false; // when it first added to list of actors we wait one fame before we start calling persist, physics world is responsible for calling this

	};
	class PhysicsSimulationCallback : public physx::PxSimulationEventCallback 
	{
	public:
		void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) {};

		// not implement we have to set them enable in actor flag
		virtual void onWake(physx::PxActor** actors, physx::PxU32 count) {};	
		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) {};
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count);

		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);

		//(Trigger actor ID, (actor that made trigger, info between trigger and acotr))
		std::unordered_map<uint64_t, std::unordered_map<uint64_t,PhysicsSimulationTriggerObject>> TriggersActors; 
	};
}
	