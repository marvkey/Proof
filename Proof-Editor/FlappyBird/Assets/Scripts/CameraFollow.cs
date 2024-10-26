
using System;
using Proof;

namespace FlappyBird
{
	public class CameraFollow : Entity
	{
		public Entity Target;

		public bool FollowX = false;
		public bool FollowY = true;
		public bool FollowZ = true;
		public Vector3 FollowOffset = new Vector3(0);
		float followSpeed = 5;
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{
		}

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
			
		}

		void OnPostUpdate(float deltaTime)
		{
            if (Target == null)
            {
                Log.Error($"{Name} Camera follow script has no target");
                return;
            }

            Vector3 position = new Vector3(Location);
            if (FollowX)
                position.x = FollowOffset.x + Target.Location.x;
            if (FollowY)
                position.y = FollowOffset.y + Target.Location.y;
            if (FollowZ)
                position.z = FollowOffset.z + Target.Location.z;

            Location = Vector3.Lerp(Location, position, followSpeed * deltaTime);
        }

	}
}
