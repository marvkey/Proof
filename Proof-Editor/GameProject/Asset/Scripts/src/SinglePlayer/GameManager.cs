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
        private float m_SpawnRate;
        void OnCreate()
        {
            if (Player == null || Obstacle == null) return;
            spawnDistanceZ = 20.0f;
            for (int i = 0; i < 10; i++)
            {
                int rand = Proof.Random.Int(1, 4);
                switch (rand)
                {
                    case 1:
                        SpawnObstacle1();
                        break;
                    case 2:
                        SpawnObstacle2();
                        break;
                    case 3:
                        SpawnObstacle1();
                        break;
                    case 4:
                        {
                            SpawnObstacle4();
                            i += 3;
                        }
                        break;
                }

            }

        }

        void OnUpdate(float ts)
        {
            if (Player == null || Obstacle == null) return;

            if (!Player.As<Player>().IsAlive())
                return;

            m_SpawnRate += ts;

            if(m_SpawnRate >= 5)
            {
                spawnDistanceZ -= 35;
                for (int i = 0; i < 10; i++)
                {
                    int rand = Proof.Random.Int(1, 4);
                    switch (rand)
                    {
                        case 1:
                            SpawnObstacle1();
                            break;
                        case 2:
                            SpawnObstacle2();
                            break;
                        case 3:
                        //    SpawnObstacle3();
                        case 4:
                            {
                                SpawnObstacle4();
                                i += 3;
                            }
                            break;
                    }

                }
                m_SpawnRate = 0;
            }
        }

        private void SpawnObstacle1()
        {
            Vector3 playerLocation = Player.GetComponent<TransformComponent>().Location;
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
            Vector3 playerLocation = Player.GetComponent<TransformComponent>().Location;
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
                    spawnTransform.Location.Y = -0.8f;
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
                    spawnTransform.Location.Y = -0.8f;
                    spawnTransform.Location.X = baseLocationX;
                    Entity ent = World.Instanciate(Obstacle, spawnTransform);

                    baseLocationZ += 1.6f;
                    baseLocationX -= 2.1f;
                }
            }
            spawnDistanceZ += 15;
        }

        private void SpawnObstacle3()
        {
            Vector3 playerLocation = Player.GetComponent<TransformComponent>().Location;
            Transform spawnTransform = new Transform();
            spawnTransform.Location.Z = playerLocation.Z + spawnDistanceZ;

                spawnTransform.Location.X = 0;
            spawnTransform.Location.Y = 1f;

            spawnTransform.Scale.Y = 1f;
            spawnTransform.Scale.Z = 1;
            spawnTransform.Scale.X = 25f;

            Entity ent = World.Instanciate(Obstacle, spawnTransform);
            spawnDistanceZ += 13;

        }

        private void SpawnObstacle4()
        {
            Vector3 playerLocation = Player.GetComponent<TransformComponent>().Location;
            Transform spawnTransform = new Transform();
            float SpawnTransformz = playerLocation.Z + spawnDistanceZ;

            spawnTransform.Scale.Y = 1;
            spawnTransform.Scale.Z = 1;
            spawnTransform.Scale.X = 1;
            spawnTransform.Location.Y = -0.8f;

            int iterations = Proof.Random.Int(4, 6);
            for(int i = 0; i < iterations; i++)
            {
                int subIteration = Proof.Random.Int(1, 3);
                for (int j = 0; j < subIteration; j++)
                {
                    spawnTransform.Location.Z = SpawnTransformz;
                    spawnTransform.Location.X = Proof.Random.Float(-23,23);
                    spawnTransform.Scale.X = Proof.Random.Float(1,3);
                    spawnTransform.Scale.Z = 1;
                    spawnTransform.Scale.Y = 1;

                    Entity ent = World.Instanciate(Obstacle, spawnTransform);
                }
                SpawnTransformz += 3;
            }
            spawnDistanceZ += 20;
        }
    }
}
