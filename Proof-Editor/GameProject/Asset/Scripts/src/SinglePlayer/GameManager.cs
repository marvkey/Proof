using Proof;
using ProofScriptCore.src.Proof.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace Game
{
    public class GameManager : Entity
    {
        public Entity Player;

        public Prefab Obstacle;
        public Prefab Plane;

        // spanw time per frame
        public float spawnWaitTime = 3000;

        private float m_TimeCounter = 0;

        public float SpawnDistance = 10;

        public float IncreasePlayerSpeed = 10;
        void OnCreate()
        {
        }

        void OnUpdate(float ts)
        {
            if (Player == null || Obstacle == null) return;

            if (!Player.As<Player>().IsAlive())
                return;
            if(m_TimeCounter == 3000)
            {
                SpawnObstacle();
                m_TimeCounter = 0;
            }
            m_TimeCounter += 100;
            
            Vector playerLocation = Player.GetComponent<TransformComponent>().Location;

            if(playerLocation.Z % 10 == 0)
            {
                Player.As<Player>().FowardForce += IncreasePlayerSpeed;
                Player.As<Player>().SideWayForce += IncreasePlayerSpeed;

                m_TimeCounter -= 30;
            }

        }
        private void SpawnObstacle()
        {
            Vector playerLocation = Player.GetComponent<TransformComponent>().Location;
            Transform spawnTransform = new Transform();
            spawnTransform.Location.Z = playerLocation.Z + SpawnDistance;
            spawnTransform.Location.X = Proof.Random.Float(-20, 20);
            spawnTransform.Location.Y = playerLocation.Y;


            spawnTransform.Scale = Proof.Random.Vector(1, 6);

            Entity ent = World.Instanciate(Obstacle, spawnTransform);
        }
    }
}
