
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

		public Prefab SpeicalEnemyRocket;
		public Prefab WarnRocketSpawn;
		public float SpawnEnemyRocketInterval = 5.0f;
		float m_SpawnEnemyRocketTimer = 0.0f;

		public Prefab PlayerHealthItem;
        public float SpawnHealthBoostInterval = 3.5f;
        float m_HealthBoostTimer = 0.0f;

		public Entity PlayerEntity;

        public GameHardState GameState
        {
            get
            {
                //return GameHardState.Hard;
                if (m_Counter > 15.0f)
                {
                    return GameHardState.Medium;
                }
                else if (m_Counter > 30.0f)
                {
                    return GameHardState.Hard;
                }
                else
                {
                    return GameHardState.Easy;
                }
            }
        }
        public enum GameHardState
        {
            Easy = 0,
            Medium = 1,
            Hard = 2
        }

        float m_Counter;

        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{

		}

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
            m_Counter += deltaTime;


            m_SpawnIntervalTimer += deltaTime;
			m_SpawnPowerUpTimer += deltaTime;
			m_SpawnEnemyRocketTimer += deltaTime;
            m_HealthBoostTimer += deltaTime;

            if (m_SpawnIntervalTimer >= SpawnInterval)
			{
				int num = Proof.Random.Int(0, Enemies.Length - 1);

				Prefab enemySpawn = Enemies[num];
                switch (GameState)
                {
                    case GameHardState.Easy:
                        m_SpawnIntervalTimer = Proof.Random.Float(0.0f, SpawnInterval * 0.5f);
                    break;
                    case GameHardState.Medium:
                        m_SpawnIntervalTimer = Proof.Random.Float(0.4f, SpawnInterval * 0.7f);
                        break;
                    case GameHardState.Hard:
                        m_SpawnIntervalTimer = Proof.Random.Float(0.6f * SpawnInterval, SpawnInterval);
                        break;
                }

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

           
            if (m_SpawnPowerUpTimer >= SpawnPowerUpInterval)
			{
                m_SpawnPowerUpTimer = 0.0f;

                switch (GameState)
                {
                    case GameHardState.Easy:
                        m_SpawnPowerUpTimer = Proof.Random.Float(0.0f, SpawnPowerUpInterval * 0.5f);
                        break;
                    case GameHardState.Medium:
                        m_SpawnPowerUpTimer = Proof.Random.Float(0.2f, SpawnPowerUpInterval * 0.8f);
                        break;
                    case GameHardState.Hard:
                        m_SpawnPowerUpTimer = Proof.Random.Float(0.4f * SpawnPowerUpInterval, SpawnPowerUpInterval);
                        break;
                }

                int index =Proof.Random.Int(0, Powerups.Length - 1);

                Prefab powerupSpawn = Powerups[index];

				float randomX = Proof.Random.Float(-SpawnRangeX, SpawnRangeX);

				Vector3 pos = new Vector3(randomX, SpawnHeight, -5.5f);
				Entity powerup = World.Instantiate(powerupSpawn, pos);

                powerup.GetComponent<RigidBodyComponent>().Gravity = false;
                powerup.GetComponent<RigidBodyComponent>().Velocity = new Vector3(0.0f, -2.0f, 0.0f);
				powerup.Transform.Scale /= 2.0f;
            }

			if(m_SpawnEnemyRocketTimer >= SpawnEnemyRocketInterval)
			{
                //m_SpawnEnemyRocketTimer = Proof.Random.Float(0.0f, SpawnEnemyRocketInterval);
                switch (GameState)
                {
                    case GameHardState.Easy:
                        m_SpawnEnemyRocketTimer = Proof.Random.Float(0.0f, SpawnEnemyRocketInterval * 0.3f);
                        break;
                    case GameHardState.Medium:
                        m_SpawnEnemyRocketTimer = Proof.Random.Float(0.2f, SpawnEnemyRocketInterval * 0.6f);
                        break;
                    case GameHardState.Hard:
                        m_SpawnEnemyRocketTimer = Proof.Random.Float(0.8f, SpawnEnemyRocketInterval * 0.6f);
                        break;
                }

                float randomX = Proof.Random.Float(-SpawnRangeX, SpawnRangeX);

				if(Proof.Random.Bool() == true)
				{
					randomX = PlayerEntity.Transform.Location.x; // so paleyr cant stay still 

                }

                Vector3 pos = new Vector3(randomX, SpawnHeight * 1.25f, -5.5f);
                Entity powerup = World.Instantiate(SpeicalEnemyRocket, pos);

                //powerup.GetComponent<RigidBodyComponent>().Gravity = false;
               // powerup.GetComponent<RigidBodyComponent>().Velocity = new Vector3(0.0f, -2.0f, 0.0f);
                powerup.Transform.Scale *= 1.5f;

				if(WarnRocketSpawn != null)
				{
                    pos.y = 0.0f;

					Entity e = World.Instantiate(WarnRocketSpawn, pos);
					e.Transform.Scale /= 2.0f;

                    World.DeleteEntity(e, true,0.75f);

                }

            }

			if(m_HealthBoostTimer >= SpawnHealthBoostInterval)
			{
				m_HealthBoostTimer = Proof.Random.Float(0.0f, SpawnHealthBoostInterval);

                switch (GameState)
                {
                    case GameHardState.Easy:
                        m_HealthBoostTimer = Proof.Random.Float(0.0f, SpawnHealthBoostInterval * 0.75f);
                        break;
                    case GameHardState.Medium:
                        m_HealthBoostTimer = Proof.Random.Float(0.0f, SpawnHealthBoostInterval * 0.5f);
                        break;
                    case GameHardState.Hard:
                        m_HealthBoostTimer = Proof.Random.Float(0.5f, SpawnHealthBoostInterval * 0.3f);
                        break;
                }

                float randomX = Proof.Random.Float(-SpawnRangeX, SpawnRangeX);

                Vector3 pos = new Vector3(randomX, SpawnHeight * 1.25f, -5.5f);
                Entity powerup = World.Instantiate(PlayerHealthItem, pos);
                powerup.Scale /= 2.0f;

                powerup.GetComponent<RigidBodyComponent>().Gravity = false;
                powerup.GetComponent<RigidBodyComponent>().Velocity = new Vector3(0.0f, -2.0f, 0.0f);
            }
        }

		// OnPhysicsUpdate is called at a fixed timestep for physics-related logic
		void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
		{
		}
      
    }
}
