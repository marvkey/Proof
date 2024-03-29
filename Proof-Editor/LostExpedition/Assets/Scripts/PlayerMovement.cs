
using System;
using Proof;

namespace LostExpedition
{
	public class PlayerMovement : Entity
	{
		public float WalkSpeed;
		public float RunSpeed;

		RigidBodyComponent m_RigidBody;
		PlayerInputComponent m_PlayerInput;

		public InputAction MoveAction;
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{
            m_RigidBody = GetComponent<RigidBodyComponent>();
			m_PlayerInput = GetComponent<PlayerInputComponent>();

			m_PlayerInput.BindAction(MoveAction, InteractionEvent.Triggered, Move);
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

		void Move(InputActionOutput outPut)
		{
			Log.Info($"WOrking {outPut.Get<Vector2>().ToString()}");

            Vector3 vetor3 = new Vector3(-outPut.Get<Vector2>().X, 0.0f, outPut.Get<Vector2>().Y);
            m_RigidBody.Location += vetor3;
        }

	}
}
