
using System;
using Proof;

namespace LostExpedition
{
	public class PlayerMovement : Entity
	{
		public float WalkSpeed;
		public float RunSpeed;

		RigidBodyComponent m_RigidBody;
		// OnCreate is called once when the Entity that this script is attached to
		// is instantiated in the world at runtime
		void OnCreate()
		{
            m_RigidBody = GetComponent<RigidBodyComponent>();

        }

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
			if (m_RigidBody == null)
			{
				Log.Error("PlayerMovement No RigidBodyComponent");
				return;
			}

		}

	}
}
