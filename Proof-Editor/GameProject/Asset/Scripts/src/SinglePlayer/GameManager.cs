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

        private float spawnDistanceZ = 0.0f;
        void OnCreate()
        {
            if (Player == null || Obstacle == null) return;
            spawnDistanceZ = 20.0f;

            for(int i=0; i < 10; i++)
            {
                int rand = Proof.Random.Int(1, 2);
                switch (rand)
                {
                    case 1:
                        SpawnObstacle1();
                        break;
                    case 2:
                        SpawnObstacle2();
                        break;
                }
            }
           
        }

        void OnUpdate(float ts)
        {
            if (Player == null || Obstacle == null) return;

            if (!Player.As<Player>().IsAlive())
                return;

        }

        private void SpawnObstacle1()
        {
            Vector playerLocation = Player.GetComponent<TransformComponent>().Location;
            Transform spawnTransform = new Transform();
            spawnTransform.Location.Z = playerLocation.Z + spawnDistanceZ;

            if(Proof.Random.Bool() == true)
             spawnTransform.Location.X =12.5f;
            else
                spawnTransform.Location.X = -12.5f;
            spawnTransform.Location.Y = 1f;

            spawnTransform.Scale.Y = 2.5f;
            spawnTransform.Scale.Z = 1;
            spawnTransform.Scale.X = 12.5f;

            Entity ent = World.Instanciate(Obstacle, spawnTransform);
            spawnDistanceZ += 13;

        }


        private void SpawnObstacle2()
        {
            Vector playerLocation = Player.GetComponent<TransformComponent>().Location;
            Transform spawnTransform = new Transform();

            spawnTransform.Scale.Y = 1;
            spawnTransform.Scale.Z = 1;
            spawnTransform.Scale.X = 1;

            float baseLocationZ = playerLocation.Z + spawnDistanceZ;
            float baseLocationX = Proof.Random.Float(-9,9);

            if(Proof.Random.Bool() == true)
            {
                for (int i = 0; i < 4; i++)
                {
                    spawnTransform.Location.Z = baseLocationZ;
                    spawnTransform.Location.Y = 1;
                    spawnTransform.Location.X = baseLocationX;
                    Entity ent = World.Instanciate(Obstacle, spawnTransform);

                    baseLocationZ += 1.6f;
                    baseLocationX += 2.1f;
                }
            }
            else
            {
                for (int i = 0; i < 4; i++)
                {
                    spawnTransform.Location.Z = baseLocationZ;
                    spawnTransform.Location.Y = 1;
                    spawnTransform.Location.X = baseLocationX;
                    Entity ent = World.Instanciate(Obstacle, spawnTransform);

                    baseLocationZ += 1.6f;
                    baseLocationX -= 2.1f;
                }
            }

            spawnDistanceZ += 15;


        }
    }
}
