#pragma once
namespace Proof
{
	class PhysxShapesInternal
	{
	public:
		static void UpdateOrAddCubeCollider(class PhysicsActor& actorator);
		static void UpdateOrAddSphereCollider(class PhysicsActor& actor);
		static void UpdateOrAddCapsuleCollider(class PhysicsActor& actor);
	};
}