
using System;
using Proof;

namespace Driftwood
{
	public class FollowXZ : Entity
	{
		public Entity Target;
		// OnCreate is called once when the Entity that this script is attached to
		// is instantiated in the world at runtime
		void OnCreate()
		{
		}

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
			if (Target == null) return;
			Vector3 position = new Vector3(Target.Location.x, Location.y, Target.Location.z);

			Location = position;
		}

	}
}
