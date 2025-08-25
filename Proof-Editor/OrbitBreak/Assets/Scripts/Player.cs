
using System;
using System.Data.SqlTypes;
using Proof;

namespace OrbitBreak
{
	public class Player : Entity
	{
		public float SideSpeed = 1.0f;
		public float Health = 100.0f;

		public float FireRate = 0.5f; // bullets per second

		float m_FireRateTimer = 0.0f;
        public Entity ChildEnityShield = null;
		public Entity SpawBullet = null;
		public Prefab Bullet = null;

		public Prefab OnHitParticle = null;
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{
			TriggerEnterEvent += OnTriggerEnter;
        }

		float speedScale = 1.0f;

        PowerUp.State m_PowerUpState
		{
			get 
			{
				if(HasScript<PowerUp>())
					return GetScript<PowerUp>().CurrentState;
                else
                    return PowerUp.State.None; 
			}
        }
        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
		{
			if(Health<= 0.0f)
			{
				Console.WriteLine("Player has been defeated!");
				World.DeleteEntity(this);
            }

			OnUpdatePowerUp();

			m_FireRateTimer += deltaTime;
            if (Input.IsKeyClicked(KeyBoardKey.Space) ||Input.IsMouseButtonClicked(MouseButton.Button0) && m_PowerUpState == PowerUp.State.Gun)
			{
				if(m_FireRateTimer >= FireRate)
				{
                    Entity e = World.Instantiate(Bullet, SpawBullet.Transform.Location);
                    e.GetComponent<RigidBodyComponent>().Velocity = new Vector3(0.0f, 2.0f, 0.0f);

					m_FireRateTimer = 0.0f;
                }
			}
        }

		void OnUpdatePowerUp()
		{
			speedScale = 1.0f; //incase dont run

			ChildEnityShield.GetComponent<MeshComponent>().Visible = false;
            if (!HasScript<PowerUp>())
				return;

            PowerUp powerUpState = GetScript<PowerUp>();

			if (!powerUpState.IsActive)
				return;


			switch (powerUpState.CurrentState)
			{
				case PowerUp.State.Shield:
				ChildEnityShield.GetComponent<MeshComponent>().Visible = true;
                    break;
				case PowerUp.State.Gun:
					// Implement gun logic
					break;
				case PowerUp.State.Speed:
					// Implement speed boost logic
					speedScale = powerUpState.SpeedScale;
                    break;
            }
        }
		// OnPhysicsUpdate is called at a fixed timestep for physics-related logic
		void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
		{
            if (Input.IsKeyPressed(KeyBoardKey.A) || Input.IsKeyPressed(KeyBoardKey.LeftArrow))
            {
                GetComponent<RigidBodyComponent>().Location += Transform.Right * -SideSpeed * fixedPhysicsDeltaTime * speedScale;
            }

            if (Input.IsKeyPressed(KeyBoardKey.D) || Input.IsKeyPressed(KeyBoardKey.RightArrow))
            {
                GetComponent<RigidBodyComponent>().Location += Transform.Right * SideSpeed * fixedPhysicsDeltaTime * speedScale;
            }
        }

		
		void OnTriggerEnter(Entity other)
		{
			Log.Info("Player Trigger Enter: " + other.ID);
            if (other.GetComponent<TagComponent>().HasSubTag("SpeedBoost"))
			{
				Log.Info("Speed Boost Collected");
                GetScript<PowerUp>().SetState(PowerUp.State.Speed); // shield broken
                other.GetComponent<MeshComponent>().Visible = false;
                return;
			}

            if (other.GetComponent<TagComponent>().HasSubTag("Shield"))
			{
                Log.Info("Shield Collected");
                GetScript<PowerUp>().SetState(PowerUp.State.Shield); // shield broken
                other.GetComponent<MeshComponent>().Visible = false;
                return;
            }

            if (other.GetComponent<TagComponent>().HasSubTag("Lazer"))
            {
                Log.Info("Lazer Collected");
                GetScript<PowerUp>().SetState(PowerUp.State.Gun); // shield broken
                other.GetComponent<MeshComponent>().Visible = false;
                return;
            }


            if (!other.GetComponent<TagComponent>().HasSubTag("Enemy"))
			return;

			other.GetComponent<MeshComponent>().Visible = false;
			
			if(m_PowerUpState == PowerUp.State.Shield)
			{
				Log.Info("Shield hit");
                GetScript<PowerUp>().SetState(PowerUp.State.None); // shield broken
            }
			else
			{
				if(OnHitParticle != null)
				{
					Entity e = World.Instantiate(OnHitParticle, Transform.Location);
					e.GetScript<DeleteEntityScript>().TargetEntity = this;
                }
                Log.Error("Removed ehalth");
				Health -= 10.0f;
			}
        }
    }
}

