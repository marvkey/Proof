
using System;
using Proof;

namespace NullState
{
	public class Drone : Entity
	{

		public Entity FlockHolder; // 
		public Entity Target;

		BoidFlockComponent flock
		{
			get
			{
				if (FlockHolder != null)
				{
					if(FlockHolder.HasComponent<BoidFlockComponent>())
						return FlockHolder.GetComponent<BoidFlockComponent>();
				}
				
				return null;
			}
		}
		
		// OnCreate is called once when the Entity that this script is attached to
		// is instantiated in the world at runtime
		void OnCreate()
		{
		}
		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
			
		}

		// OnPhysicsUpdate is called at a fixed timestep for physics-related logic
		void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
		{
			if (Target != null)
			{
				var heading = Target.Location - Transform.Location;

				GetComponent<RigidBodyComponent>().Rotation = Quaternion.LookRotation(heading, Mathf.Up);
			}
		}

		void Shoot()
		{
			if (flock == null)
				return;
			
		}
	}
}
