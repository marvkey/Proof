
using System;
using Proof;

namespace OrbitBreak
{
	public class SpawnManager : Entity
	{
		public float SpawnInterval = 2.0f;
		float m_SpawnIntervalTimer = 0.0f;

        public float SpawnPowerUpInterval = 2.5f;
		float m_SpawnPowerUpTimer = 0.0f;

        public float  SpawnHeight= 5.0f;

		public float SpawnRangeX = 3.7f;

		public Prefab[] Enemies;
		public Prefab[] Powerups;

        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{

		}

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
			m_SpawnIntervalTimer += deltaTime;
			m_SpawnPowerUpTimer += deltaTime;

            if (m_SpawnIntervalTimer >= SpawnInterval)
			{
				int num = Proof.Random.Int(0, Enemies.Length - 1);

				Prefab enemySpawn = Enemies[num];
                m_SpawnIntervalTimer = Proof.Random.Float(0.0f, SpawnInterval);

				float randomX = Proof.Random.Float(-SpawnRangeX, SpawnRangeX);

				Vector3 pos = new Vector3(randomX,SpawnHeight,-5.5f);
                Entity enemy = World.Instantiate(enemySpawn, pos);
				enemy.Transform.RotationQuat = Quaternion.EularToQuat(new Vector3(Proof.Random.Float(0,360),
                    Proof.Random.Float(0, 360),
                    Proof.Random.Float(0, 360)));
                enemy.Scale /= 2.0f;
				enemy.GetComponent<RigidBodyComponent>().Gravity = false;
				enemy.GetComponent<RigidBodyComponent>().Velocity = new Vector3(0.0f, -2.0f, 0.0f);
            }

			if(m_SpawnPowerUpTimer >= SpawnPowerUpInterval)
			{
                m_SpawnPowerUpTimer = 0.0f;

				int index =Proof.Random.Int(0, Powerups.Length - 1);

				Prefab powerupSpawn = Powerups[index];

				float randomX = Proof.Random.Float(-SpawnRangeX, SpawnRangeX);

				Vector3 pos = new Vector3(randomX, SpawnHeight, -5.5f);
				Entity powerup = World.Instantiate(powerupSpawn, pos);

                powerup.GetComponent<RigidBodyComponent>().Gravity = false;
                powerup.GetComponent<RigidBodyComponent>().Velocity = new Vector3(0.0f, -2.0f, 0.0f);
				powerup.Transform.Scale /= 2.0f;
            }
        }

		// OnPhysicsUpdate is called at a fixed timestep for physics-related logic
		void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
		{
		}
	}
}
