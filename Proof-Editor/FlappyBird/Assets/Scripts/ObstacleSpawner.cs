
using System;
using System.Collections.Generic;
using Proof;

namespace FlappyBird
{
	public class ObstacleSpawner : Entity
	{
		public Entity Bird;
		public float DistanceFromPlayer = 10;
		public float Frequency = 2;
		float m_Timer;
		public Prefab Obstacle;
		float m_LastPositionSpawn = 0;
		List<Entity> m_SpawnObstacles = new List<Entity>();
		// OnCreate is called once when the Entity that this script is attached to
		// is instantiated in the world at runtime
		void OnCreate()
		{
			if (Obstacle == null)
				return;
			m_Timer = Frequency;

			for (int i = 0; i < 5; i++)
			{
				SpawnObstacle();
			}

        } 

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
			if (Bird == null)
				return;

            if (Obstacle == null)
                return;

			if (Bird.GetScript<BirdMovement>().GetState() == BirdMovement.BirdState.Dead)
				return;
			m_Timer -= deltaTime;

			if(m_Timer <= 0)
			{
				SpawnObstacle();
				m_Timer = Frequency;
            }
			for(int i = 0; i< m_SpawnObstacles.Count; i++)
			{
				if (CanDeleteObstacle(m_SpawnObstacles[i]))
				{
					Entity obstacle = m_SpawnObstacles[i];
					Log.Info($"DELETED OBSTACLE {obstacle.Name}");
					World.DeleteEntity(m_SpawnObstacles[i]);
                    m_SpawnObstacles.RemoveAt(i);
				}
				else
				{
					// we looping in order so at the back is the furtherst obstacles from the plaeyr
					break;
				}
			}
        }

		bool CanDeleteObstacle(Entity obstacle)
		{
			if(Bird ==null)
				return false;

			if(obstacle.Location.x > Bird.Location.x + 50)
			{
				return true;
			}
			return false;
		}
		void SpawnObstacle()
		{
			if (m_LastPositionSpawn == 0)
				m_LastPositionSpawn = Bird.Transform.Location.x + DistanceFromPlayer;
			else
				m_LastPositionSpawn += DistanceFromPlayer;
            Vector3 spawnPos = new Vector3(m_LastPositionSpawn, Proof.Random.Float(5,15), 0);
			Entity newObstacle = World.Instantiate(Obstacle, spawnPos);
            m_SpawnObstacles.Add(newObstacle);
        }

    }
}
