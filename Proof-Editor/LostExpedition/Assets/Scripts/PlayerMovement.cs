
using System;
using Proof;

namespace LostExpedition
{
    public enum MoveState
    {
        Walking,
        Running
    }
    public class PlayerMovement : Entity
	{
		public float WalkSpeed = 25.0f;
        public float RunSpeed = 50.0f;
		public float JumpForce = 16.0f;
		RigidBodyComponent m_RigidBody;
		PlayerInputComponent m_PlayerInput;
		
		[ShowInEditorAttribute("MovingState",true)]
		MoveState m_MovingState = MoveState.Walking;

		public InputAction MoveAction;
		public InputAction LookAction;
		public InputAction TriggerFastMove;
		public InputAction JumpAction;
		public Entity Head;
		// OnCreate is called once when the Entity that this script is attached to
		// is instantiated in the world at runtime
		void OnCreate()
		{
			m_RigidBody = GetComponent<RigidBodyComponent>();
			m_PlayerInput = GetComponent<PlayerInputComponent>();

			//if (m_RigidBody == null || m_PlayerInput == null)
			//	return;
			if (MoveAction != null)
				m_PlayerInput.BindAction(MoveAction, InteractionEvent.Triggered, Move);

			if(TriggerFastMove != null)
				m_PlayerInput.BindAction(TriggerFastMove, InteractionEvent.Triggered, EnableRunning);

			if(LookAction != null)
				m_PlayerInput.BindAction(LookAction, InteractionEvent.Triggered, Look);

			if(JumpAction != null) m_PlayerInput.BindAction(JumpAction, InteractionEvent.Triggered, Jump);

			Mouse.SetCursorMode(MouseCursorMode.Locked);
        }

        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
		{
			if (m_RigidBody == null)
			{
				Log.Error($"PlayerMovementScript {this.Name} needs a RigidBody Attached");
				return;
			}
			if (m_PlayerInput == null)
			{
                Log.Error($"PlayerMovementScript {this.Name} needs a PlayerInput Attached");
				return;
            }

			if (Input.IsKeyClicked(KeyBoardKey.M)) // this is for debug purposes 
				Mouse.SetCursorMode(MouseCursorMode.Normal); 
		}

		void Move(InputActionOutput actionOutput)
		{
			float moveSpeed = m_MovingState == MoveState.Walking ? WalkSpeed : RunSpeed;

			Vector2 axisData = actionOutput.Get<Vector2>();
			
			Vector3 movement = Transform.Forward * axisData.y * moveSpeed * World.GetDeltaTime();
			Transform.Location += movement;

			movement = Transform.Right * axisData.x * moveSpeed * World.GetDeltaTime();
			Transform.Location += movement;
        }

		void Look(InputActionOutput actionOutput)
		{
            Vector2 axisData = actionOutput.Get<Vector2>();

			float rotateSpeed = 2.0f;

			Transform.Rotation = new Vector3(Transform.Rotation.x, Transform.Rotation.y + World.GetDeltaTime() * -axisData.x * rotateSpeed, Transform.Rotation.z);

			Vector3 e = Head.Transform.Rotation;

			e.x += axisData.y * World.GetDeltaTime() * rotateSpeed;
			e.x = RestrictAngle(e.x, -85, 85);
			Head.Transform.Rotation = e;

        }

        void EnableRunning(InputActionOutput actionOutput)
		{
			if(m_MovingState == MoveState.Running)
				m_MovingState = MoveState.Walking;
			else
				m_MovingState = MoveState.Running;
		}

		bool IsGrounded()
		{
			bool val = Physics.Raycast(Location * new Vector3(0.0f, -1.0f, 1.0f) * 0.25f, new Vector3(0.0f, -1.0f, 1.0f), 1.0f, out RaycastHit hitInfo);
			//if (val)
			//	Log.Info($"Entity grounded touching {hitInfo.Entity.Name}");

			return val;
		}
		void Jump(InputActionOutput actionOutput)
		{
			if(IsGrounded())
			{
				m_RigidBody.AddForce(new Vector3(0, 1.0f, 0).Normalized * JumpForce, ForceMode.Impulse);
			}
		}

		public static float RestrictAngle(float angle, float angleMin, float angleMax)
		{
			if (angle > 180)
				angle -= 360;
			else if (angle < -180)
				angle += 360;

			if (angle > angleMax)
				angle = angleMax;

			if(angle < angleMin)
				angle = angleMin;

			return angle;
		}

	}
}
