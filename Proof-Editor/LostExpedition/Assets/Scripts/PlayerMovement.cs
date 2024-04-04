
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

		public Vector3 Test;
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

			if (outPut.Get<Vector2>().y != 0)
			{
				Vector3 movement = Transform.Forward * outPut.Get<Vector2>().y * moveSpeed * World.GetDeltaTime();
				Transform.Location += movement;
			}
			if (outPut.Get<Vector2>().x != 0)
			{
                Vector3 movement = Transform.Right * outPut.Get<Vector2>().x * moveSpeed * World.GetDeltaTime();
                Transform.Location += movement;
			}


        }
		void Rotate(InputActionOutput outPut)
		{
			Vector2 axisData = outPut.Get<Vector2>();

			float rotateSpeed = 100.5f;


			if(axisData.x != 0)
			{
				//Transform.Rotation.Y += World.GetDeltaTime() * axisData.X;
				Transform.Rotation = new Vector3(Transform.Rotation.x, Transform.Rotation.y + World.GetDeltaTime() * -axisData.x, Transform.Rotation.z);


              //Transform.Rotate(Vector3.Up * -axisData.X );   // Adjust the multiplier for different rotation speed
            }
            if(axisData.y != 0)
			{
                Vector3 e = Head.Transform.Rotation;
				e.x += axisData.y * World.GetDeltaTime();
				e.x = RestrictAngle(e.x, -85, 85);
                Head.Transform.Rotation = e;
			}

        }
        void MoveFast(InputActionOutput outPut)
		{
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
