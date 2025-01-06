
using System;
using System.Collections.Generic;
using System.Linq;
using Proof;

namespace FlappyBird
{
	public class PlatformSpawn : Entity
	{
		public Prefab Obstacle;

        float m_LastSpawnX = 0;
		float Frequency = 15;
		float DistanceFromLast = -112;
        float m_Timer;
        float m_DeleteTimer =60;
        List<Entity> m_SpawnPlatforms = new List<Entity>();

        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{
            m_Timer = Frequency;
            for (int i = 0; i < 2; i++)
            {
                SpawnPlatform();
            }
            m_DeleteTimer = 60;
        }

        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
		{

            m_Timer -= deltaTime;

            if (m_Timer <= 0)
            {
                SpawnPlatform();
                m_Timer = Frequency;
            }
            if (m_DeleteTimer <=0)
            {
                Entity obstacle = m_SpawnPlatforms[0];
                Log.Info($"DELETED Platform {obstacle.Name}");
                World.DeleteEntity(obstacle);
                m_SpawnPlatforms.RemoveAt(0);
                m_DeleteTimer = 60;
            }
            m_DeleteTimer -= deltaTime;


        }
         
		void SpawnPlatform()
		{
			if(Obstacle ==null) return;

            Vector3 spawnPos = new Vector3(m_LastSpawnX, -5f, 0);
            Entity newObstacle = World.Instantiate(Obstacle, spawnPos);
            m_SpawnPlatforms.Add(newObstacle);
            m_LastSpawnX += DistanceFromLast;
        }

        bool CanDeleteObstacle(Entity obstacle)
        {
            if (Mathf.Abs(m_LastSpawnX) > Mathf.Abs(obstacle.Location.x * 2))
                return true;

            return false;
        }
    }
}
