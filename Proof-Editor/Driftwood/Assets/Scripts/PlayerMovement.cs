
using System;
using System.Security.AccessControl;
using Proof;

namespace Driftwood
{
	public class PlayerMovement : Entity
	{
		public float FowardForce = 100.0f;
		RigidBodyComponent m_RigidBody;
		// OnCreate is called once when the Entity that this script is attached to
		// is instantiated in the world at runtime

		private float m_MoveSideWaysAccumlator = 0.0f;
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
         
        }

		void OnPhysicsUpdate(float fixedPhysicsetlaTime)
		{
            if (m_RigidBody == null)
            {
                Log.Error($"{Name} Requires a RigidBoyd for PlayerMovement script to work");
                return;
            }

            m_RigidBody.AddForce(Transform.Forward * FowardForce * fixedPhysicsetlaTime, ForceMode.Acceleration);

			
			if(m_MoveSideWaysAccumlator != 0.0f)
				m_RigidBody.AddForce(Transform.Right * (FowardForce * 2) * fixedPhysicsetlaTime * m_MoveSideWaysAccumlator, ForceMode.Force);

        }


        public void MoveSideways(float x)
		{
            m_MoveSideWaysAccumlator += x;
		}

	}
}
