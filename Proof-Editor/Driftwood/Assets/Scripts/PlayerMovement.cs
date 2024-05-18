
using System;
using Proof;

namespace Driftwood
{
	public class PlayerMovement : Entity
	{
		public float FowardForce = 100.0f;
		RigidBodyComponent m_RigidBody;
		// OnCreate is called once when the Entity that this script is attached to
		// is instantiated in the world at runtime
		void OnCreate()
		{
			m_RigidBody = GetComponent<RigidBodyComponent>();

			if(m_RigidBody == null)
			{
				Log.Error($"{Name} Requires a RigidBoyd for PlayerMovement script to work");
				return;
			}

        }

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
            if (m_RigidBody == null)
            {
                Log.Error($"{Name} Requires a RigidBoyd for PlayerMovement script to work");
                return;
            }

			m_RigidBody.AddForce(Transform.Forward * FowardForce * deltaTime, ForceMode.Force);
        }

		public void MoveSideways(float x)
		{
			m_RigidBody.AddForce(Transform.Right * (FowardForce * 2) * World.GetDeltaTime() * x, ForceMode.Force);	
		}

	}
}
