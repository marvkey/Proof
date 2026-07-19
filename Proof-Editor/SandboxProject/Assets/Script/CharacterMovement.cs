
using System;
using System.Linq;
using System.Threading;
using Proof;

namespace Proof
{
	public class CharacterMovement : Entity
	{
        public float WalkingSpeed = 10;
        public float RunningSpeed = 20;

		public float RotationSpeed = 10;
        public float JumpForce = 5;

        public bool IsRunning = false;

		RigidBodyComponent m_RigidBody;
		public Entity Camera;

		Vector2 moveRate = Vector2.Zero;
		Vector2 rotateRate = Vector2.Zero;

		float verticalRotation;
        PlayerHUDComponent m_HudComponent;
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{
            m_RigidBody = GetComponent<RigidBodyComponent>();
            m_HudComponent = GetComponent<PlayerHUDComponent>();

            if (m_RigidBody == null)
				Log.Error($"{Name} needs a RigidBodyComponent");

            if (Camera != null)
			{
				if(!Camera.HasComponent<CameraComponent>())
                    Log.Error($"{Name} Camera Entity needs a CameraComponent");
            }
			verticalRotation = Camera.Rotation.z;

            Mouse.SetCursorMode(MouseCursorMode.Locked);
            


        }
        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
		{

           

        }

        // OnPhysicsUpdate is called at a fixed timestep for physics-related logic
        void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
		{
			if (m_RigidBody == null)
				return;

			if(moveRate != Vector2.Zero)
			{

                // Combine into one direction vector
                Vector3 moveDir = (Transform.Forward * moveRate.y) + (Transform.Right * moveRate.x);

                // Normalize to prevent diagonal speed boost
                // usign length squred make sure its not lenght is not 0
                if (moveDir.LengthSqr > 0f)
                    moveDir = moveDir.Normalized;

                // Apply movement
                m_RigidBody.Location += moveDir * (IsRunning ? RunningSpeed : WalkingSpeed) * fixedPhysicsDeltaTime;


               // m_RigidBody.Location += Transform.Forward * (IsRunning ? RunningSpeed : WalkingSpeed) * fixedPhysicsDeltaTime * moveRate.y;
               // m_RigidBody.Location += Transform.Right * (IsRunning ? RunningSpeed : WalkingSpeed) * fixedPhysicsDeltaTime * moveRate.x;
                moveRate = Vector2.Zero;
            }

            if (rotateRate != Vector2.Zero)
            {
                m_RigidBody.Rotate(new Quaternion(new Vector3(0, Mathf.DegreesToRadians(-(rotateRate.x * RotationSpeed * fixedPhysicsDeltaTime)), 0)));


                verticalRotation += rotateRate.y * RotationSpeed * fixedPhysicsDeltaTime;
                verticalRotation = Mathf.Clamp(verticalRotation, -90, 90);
                {
                    // disbale up down rotation
                    Camera.Transform.RotationQuat = Quaternion.EularToQuat(new Vector3(Mathf.DegreesToRadians(verticalRotation), 0, 0));
                }
                rotateRate = Vector2.Zero;
            }
        }

		public void Move(Vector2 axis)
		{
			moveRate += axis;
        }

        public void Rotate(Vector2 axis)
		{
			rotateRate += axis;
        }

        public void Jump()
        {
            if(IsGrounded())
            {
                Log.Trace("Jumped");

                m_RigidBody.AddForce(Mathf.Up * JumpForce,ForceMode.Impulse);
            }
        }
        bool IsGrounded()
        {

            RaycastData data = new RaycastData();
            data.Origin = Transform.Location;
            data.Direction = Mathf.Down;
            data.MaxDistance = 1.1f;


            data.ExcludedEntities = new ulong[1];
            data.ExcludedEntities[0] = ID;

            if (Physics.RayCast(data, out RaycastHit hit))
            {
                return true;
            }

            return false;
        }

    }
}
