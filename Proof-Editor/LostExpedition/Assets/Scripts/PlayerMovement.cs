
using System;
using System.Threading;
using Proof;

namespace LostExpedition
{
	public class PlayerMovement : Entity
	{

		public float WalkSpeed = 10.0f;
		public float RunSpeed = 20.0f;
		public Entity Head;

		RigidBodyComponent m_RigidBody;
		PlayerInputComponent m_PlayerInput;

		public InputAction MoveAction;
		public InputAction RotateAction;
		public InputAction MoveFastAction;

        enum MoveState
		{
			Walking,
			Running
		}
        MoveState m_MoveState = MoveState.Walking;
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{
            m_RigidBody = GetComponent<RigidBodyComponent>();
			m_PlayerInput = GetComponent<PlayerInputComponent>();

			m_PlayerInput.BindAction(MoveAction, InteractionEvent.Triggered, Move);
            m_PlayerInput.BindAction(RotateAction, InteractionEvent.Triggered, Rotate);
            m_PlayerInput.BindAction(MoveFastAction, InteractionEvent.Completed, MoveFast);

            Mouse.SetCursorMode(MouseCursorMode.Locked);
        }

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
			if (m_RigidBody == null)
			{
				Log.Error("PlayerMovement No RigidBodyComponent");
				return;
			}
			if (Input.IsKeyClicked(KeyBoardKey.Space))
			{
				Mouse.SetCursorMode(MouseCursorMode.Normal);
			}
		}

		void Move(InputActionOutput outPut)
		{
			float moveSpeed;
			if (m_MoveState == MoveState.Walking)
				moveSpeed = WalkSpeed;
			else
				moveSpeed = RunSpeed;

			Vector2 axisData = outPut.Get<Vector2>();

			if (outPut.Get<Vector2>().Y != 0)
			{
				Vector3 movement = Transform.Forward * outPut.Get<Vector2>().Y * moveSpeed * World.GetDeltaTime();
				m_RigidBody.Location += movement;
			}
			if (outPut.Get<Vector2>().X != 0)
			{
                Vector3 movement = Transform.Right * outPut.Get<Vector2>().X * moveSpeed * World.GetDeltaTime();
                m_RigidBody.Location += movement;
			}


        }
		void Rotate(InputActionOutput outPut)
		{
			Vector2 axisData = outPut.Get<Vector2>();

			float rotateSpeed = 100.5f;

			//Log.Info($"ROtation {outPut.Get<Vector2>().ToString()}");

			if(axisData.X != 0)
			{
				 
              Transform.Rotate(Vector3.Up * -axisData.X );   // Adjust the multiplier for different rotation speed
			}
            if(axisData.Y != 0)
			{
                Vector3 e = Head.Transform.Rotation;
				e.X += axisData.Y;
				e.X = RestrictAngle(e.X, -85, 85);
                Head.Transform.Rotation = e;
			}

        }
        void MoveFast(InputActionOutput outPut)
		{
			Log.Info($"Move Fast");
            if (m_MoveState == MoveState.Walking)
				m_MoveState = MoveState.Running;
			else
				m_MoveState = MoveState.Walking;
        } 

        public static float RestrictAngle(float angle, float angleMin, float angleMax)
		{
			if (angle > 180)
				angle -= 360;
			else if (angle < -180)
				angle += 360;

			if (angle > angleMax)
				angle = angleMax;

			if(angle <angleMin)
				angle = angleMin;

			return angle;
		}

    }
}
