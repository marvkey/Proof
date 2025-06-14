
using System;
using System.Linq;
using System.Threading;
using Proof;

namespace LostExpedition
{
	public class PlayerMovement : Entity
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
        float beginVal = 25;
        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
		{

            if (rotateRate != Vector2.Zero)
            {
                m_RigidBody.Rotate(new Quaternion(new Vector3(0, Mathf.DegreesToRadians(-(rotateRate.x * RotationSpeed * deltaTime)), 0)));


                verticalRotation -= rotateRate.y * RotationSpeed * deltaTime;
                verticalRotation = Mathf.Clamp(verticalRotation, -90, 90);
                {
                    // disbale up down rotation
                    //Camera.Transform.RotationQuat = Quaternion.EularToQuat(new Vector3(Mathf.DegreesToRadians(verticalRotation), 0, 0));
                }
                rotateRate = Vector2.Zero;
            }

            GetScript<HealthComponent>().TakeDamage(deltaTime * beginVal);

            if (m_HudComponent != null)
            {
                Variable var = m_HudComponent.GetRegistryVariableByPanelIndex(3, 0, "HealthPercent");
                
                if(var != null)
                {
                    var.SetData<float>(GetScript<HealthComponent>().GetHealth() / GetScript<HealthComponent>().GetMaxHealth());
                }
            }

            if(GetScript<HealthComponent>().GetHealth() <= 0)
            {
                World.TryFindEntityByTag("Empty Entity").GetComponent<TextComponent>().Visible = true;
                GetComponent<MeshComponent>().Visible = false;
            }

        }

        // OnPhysicsUpdate is called at a fixed timestep for physics-related logic
        void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
		{
			if (m_RigidBody == null)
				return;

			if(moveRate != Vector2.Zero)
			{
                m_RigidBody.Location += Transform.Forward * (IsRunning ? RunningSpeed : WalkingSpeed) * fixedPhysicsDeltaTime * moveRate.y;
                m_RigidBody.Location += Transform.Right * (IsRunning ? RunningSpeed : WalkingSpeed) * fixedPhysicsDeltaTime * moveRate.x;
                moveRate = Vector2.Zero;
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
                Log.Info("Jumped");

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
