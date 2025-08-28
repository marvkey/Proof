
using System;
using System.Data.SqlTypes;
using Proof;
using System.IO;

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
		public Entity Camera;

        public Prefab ExpolsionParticle = null;
		public Prefab PowerUpSound = null;
		public Prefab PickUpHealthSound = null;
		public Prefab HurtSound = null;

        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{
			TriggerEnterEvent += OnTriggerEnter;
        }

		float speedScale = 1.0f;

		float m_Score = 0.0f;

		public float Score
		{
			get { return m_Score; }
        }
        public PowerUp.State PowerUpState
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
            Health = Mathf.Clamp(Health, 0.0f, 100.0f);

            OnUpdatePowerUp();

			m_FireRateTimer += deltaTime;
            if (Input.IsKeyClicked(KeyBoardKey.Space) ||Input.IsMouseButtonClicked(MouseButton.Button0) && PowerUpState == PowerUp.State.Gun)
			{
				if(m_FireRateTimer >= FireRate)
				{
                    Vector3 loc = SpawBullet.Transform.WorldTransform.Location;
                    loc.z = -5.5f;
                    Entity e = World.Instantiate(Bullet, loc);
                    e.GetComponent<RigidBodyComponent>().Velocity = new Vector3(0.0f, 8.0f, 0.0f);

					m_FireRateTimer = 0.0f;
                }
			}

			m_Score += deltaTime;
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


            if(GetComponent<RigidBodyComponent>().Location.x <= -4.372f)
            {
                GetComponent<RigidBodyComponent>().Location = new Vector3(-4.372f, GetComponent<RigidBodyComponent>().Location.y, GetComponent<RigidBodyComponent>().Location.z);
            }

            if (GetComponent<RigidBodyComponent>().Location.x >= 4.372f)
            {
                GetComponent<RigidBodyComponent>().Location = new Vector3(4.372f, GetComponent<RigidBodyComponent>().Location.y, GetComponent<RigidBodyComponent>().Location.z);
            }
        }

		
        void SpawnSounds(Prefab sound)
        {
            if (sound == null)
                return;

            Entity e = World.Instantiate(sound, Transform.Location);
            World.DeleteEntity(e, true, 2.0f);
        }
		void OnTriggerEnter(Entity other)
		{
            if (other.GetComponent<TagComponent>().HasSubTag("SpeedBoost"))
			{
				Log.Info("Speed Boost Collected");
                GetScript<PowerUp>().SetState(PowerUp.State.Speed); // shield broken
                other.GetComponent<MeshComponent>().Visible = false;

                SpawnSounds(PowerUpSound);
                return;
			}

            if (other.GetComponent<TagComponent>().HasSubTag("Shield"))
			{
                Log.Info("Shield Collected");
                GetScript<PowerUp>().SetState(PowerUp.State.Shield); // shield broken
                other.GetComponent<MeshComponent>().Visible = false;

                SpawnSounds(PowerUpSound);

                return;
            }

            if (other.GetComponent<TagComponent>().HasSubTag("Lazer"))
            {
                Log.Info("Lazer Collected");
                GetScript<PowerUp>().SetState(PowerUp.State.Gun); // shield broken
                other.GetComponent<MeshComponent>().Visible = false;

                SpawnSounds(PowerUpSound);
                return;
            }

            if (other.GetComponent<TagComponent>().HasSubTag("Health"))
            {
                Log.Info("health Collected");
                other.GetComponent<MeshComponent>().Visible = false;

                SpawnSounds(PickUpHealthSound);
                Health += 15;
                return;
            }


            if (!other.GetComponent<TagComponent>().HasSubTag("Enemy"))
			return;

			other.GetComponent<MeshComponent>().Visible = false;
			
			if(PowerUpState == PowerUp.State.Shield)
			{
				Log.Info("Shield hit");
                GetScript<PowerUp>().SetState(PowerUp.State.None); // shield broken
            }
			else
			{

                Log.Error("Removed ehalth");
				if(other.GetComponent<TagComponent>().HasSubTag("EnemyRocket"))
				{
                    if (ExpolsionParticle != null)
                    {
                        Entity e = World.Instantiate(ExpolsionParticle, Transform.Location);
                        e.GetScript<DeleteEntityScript>().TargetEntity = this;

						World.DeleteEntity(e, true, 1.0f);
                    }
                    Health -= 30.0f;

                    if (Camera != null)
                        Camera.GetScript<Camera>().Shake(0.5f, 0.5f);
                }
				else
				{
                    if (Camera != null)
                        Camera.GetScript<Camera>().Shake(0.25f, 0.25f);

                    SpawnSounds(HurtSound);
                    Health -= 10.0f;
				}
			}
        }
    }
}

