using Proof;
using ProofScriptCore.src.Proof.Math;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Game
{
    enum SpawnPosition
    {
        Left,
        Middle1,
        Middle2,
        Right
    }
    class mGameManager : Entity
    {
   
        public Prefab Obstacle;
        public Prefab DorrOpenerThing;
        public Prefab RampObstacle;
        public Prefab PhysicsCube;

        public Prefab Plane;
        public int WorldLenght = 1000;

        public Prefab PowerUp1;
        public Prefab PowerUp2;
        public Prefab PowerUp3;
       // public Prefab PowerUp4;
       // public Prefab PowerUp5;

        private List<Prefab> PowerUps = new List<Prefab>();

        public void OnCreate()
        {
            if (Obstacle == null)
                return;
            Transform spawnTransform = new Transform();
            spawnTransform.Scale = new Vector(100, 1, WorldLenght);
            World.Instanciate(Plane, spawnTransform);

            PowerUps.Add(PowerUp1);
            PowerUps.Add(PowerUp2);
            PowerUps.Add(PowerUp3);
            for (int i = 80; i < WorldLenght; i+= Proof.Random.Int(80,120))
            {
                switch(Proof.Random.Int(2, 2)) 
                {
                    case 1:
                        {
                            int numBlocks = Proof.Random.Int(1, 3);
                            int iteration = Proof.Random.Int(0, 4);

                            List<SpawnPosition> spawnPositions = new List<SpawnPosition>();
                            for (int j = 0; j < iteration; j++)
                            {
                                back:
                                SpawnPosition pos = (SpawnPosition)Proof.Random.Int(0, 4);
                                if (spawnPositions.Contains(pos))
                                    goto back;
                                spawnPositions.Add(pos);
                                SpawnRamp(i, pos, numBlocks);

                            }
                            i += 200 * numBlocks;
                            break;
                        }
                    case 2:
                        {
                            List<SpawnPosition> spawnPositions = new List<SpawnPosition>();
                            int iterate = Proof.Random.Int(2, 4);

                            for (int j = 0; j < iterate; j++)
                            {
                            back:
                                SpawnPosition pos = (SpawnPosition)Proof.Random.Int(0, 4);
                                if (spawnPositions.Contains(pos))
                                    goto back;
                                spawnPositions.Add(pos);

                                switch (Proof.Random.Int(1, 2))
                                {
                                    case 1:
                                        SpawnObstacle1(i, pos);
                                        break;
                                    case 2:
                                        SpawnDorrOpenThing(i, pos);
                                        break;

                                }
                            }
                            i += 20;
                            break;
                        }
                }
            }
        }
        // supports all of them
        private void SpawnObstacle1(int locationZ,SpawnPosition pos)
        {
            Transform spawnTransform = new Transform();
            spawnTransform.Location.Z = locationZ;

            switch (pos)
            {
                case SpawnPosition.Left:
                    spawnTransform.Location.X = 75f;
                    break;
                case SpawnPosition.Middle1:
                    spawnTransform.Location.X = 25;
                    break;
                case SpawnPosition.Middle2:
                    spawnTransform.Location.X = -25;
                    break;
                case SpawnPosition.Right:
                    spawnTransform.Location.X = -75f;
                    break;
                default:
                    return;
            }
            spawnTransform.Location.Y = 2.5f;

            spawnTransform.Scale.Y = 1.25f;
            spawnTransform.Scale.Z = 1;
            spawnTransform.Scale.X = 25f;

            Entity ent = World.Instanciate(Obstacle, spawnTransform);
            //PowerUp
            {
                Transform powerUPTransform = new Transform();
                powerUPTransform.Location.X = spawnTransform.Location.X;
                powerUPTransform.Location.Y = spawnTransform.Location.Y +6.5f;
                powerUPTransform.Location.Z = spawnTransform.Location.Z;

                powerUPTransform.Scale = new Vector(3);

                SpawnPowerUp(powerUPTransform);
            }
        }
        // oy supports left, and right
        private void SpawnObstacle2(int locationZ, SpawnPosition pos)
        {
            Transform spawnTransform = new Transform();
            spawnTransform.Location.Z = locationZ;

            switch (pos)
            {
                case SpawnPosition.Right:
                    spawnTransform.Location.X = -50f;
                    break;
                case SpawnPosition.Left:
                    spawnTransform.Location.X = 50f;
                    break;
                default:
                    return;
            }
            spawnTransform.Location.Y = 2.5f;

            spawnTransform.Scale.Y = 1.25f;
            spawnTransform.Scale.Z = 1;
            spawnTransform.Scale.X = 50f;

            Entity ent = World.Instanciate(Obstacle, spawnTransform);
        }
        // does not support middle
        private void SpawnDorrOpenThing(int locationZ, SpawnPosition pos)
        {
            Transform spawnTransform = new Transform();
            spawnTransform.Location.Z = locationZ;
            switch (pos)
            {
                case SpawnPosition.Left:
                    spawnTransform.Location.X = 75;
                    break;
                case SpawnPosition.Middle1:
                    spawnTransform.Location.X = 25;
                    break;
                case SpawnPosition.Middle2:
                    spawnTransform.Location.X = -25;
                    break;
                case SpawnPosition.Right:
                    spawnTransform.Location.X = -75;
                    break;
                default:
                    return;
            }
            spawnTransform.Location.Y = 11.8f;

            spawnTransform.Rotation.Z = -90;


            spawnTransform.Scale.X = 10;
            spawnTransform.Scale.Y = 25;
            spawnTransform.Scale.Z = 1;
            Entity ent = World.Instanciate(DorrOpenerThing, spawnTransform);

            //PowerUp
            {
                Transform powerUPTransform = new Transform();
                powerUPTransform.Location.X = spawnTransform.Location.X;
                powerUPTransform.Location.Y = spawnTransform.Location.Y-7;
                powerUPTransform.Location.Z = spawnTransform.Location.Z;

                powerUPTransform.Scale = new Vector(3);

                SpawnPowerUp(powerUPTransform);
            }
        }
        PowerUp SpawnPowerUp(Transform transform)
        {
            Prefab powerup = PowerUps[Proof.Random.Int(0,PowerUps.Count-1)];

            Entity entity = World.Instanciate(powerup, transform);
            return entity.As<PowerUp>();
        }
        //does not support middle
        private void SpawnRamp(int locationZ, SpawnPosition pos, int numBlocks =1)
        {
           
            Transform spawnTransform = new Transform();
            spawnTransform.Location.Z = locationZ;
            spawnTransform.Location.Y = 14.5f;
            spawnTransform.Rotation.Y = -180;

            switch (pos)
            {
                case SpawnPosition.Left:
                    spawnTransform.Location.X = 75;
                    break;
                case SpawnPosition.Middle1:
                    spawnTransform.Location.X = 25;
                    break;
                case SpawnPosition.Middle2:
                    spawnTransform.Location.X = -25;
                    break;
                case SpawnPosition.Right:
                    spawnTransform.Location.X = -75;
                    break;
                default:
                    return;
            }

            spawnTransform.Scale.X = 10.4f;
            spawnTransform.Scale.Y = 7;
            spawnTransform.Scale.Z = 7;
            Entity ent = World.Instanciate(RampObstacle, spawnTransform);
            //PowerUp
            {
                Transform powerUPTransform = new Transform();
                powerUPTransform.Location.X = spawnTransform.Location.X;
                powerUPTransform.Location.Y = spawnTransform.Location.Y + 5.5f;
                powerUPTransform.Location.Z = spawnTransform.Location.Z;

                powerUPTransform.Scale = new Vector(3);

                SpawnPowerUp(powerUPTransform);
            }
            float locationspawnCUbeZ = locationZ;
            //spawn Cu
            for (int i = 0; i < numBlocks; i++)
            {
                if (i > 0)
                {
                    locationspawnCUbeZ += 200;
                }
                else
                {
                    locationspawnCUbeZ += 124.6f;

                }
                Transform cubeTransform = new Transform();
                switch (pos)
                {
                    case SpawnPosition.Left:
                        cubeTransform.Location.X = 75;
                        break;
                    case SpawnPosition.Middle1:
                        cubeTransform.Location.X = 25;
                        break;
                    case SpawnPosition.Middle2:
                        cubeTransform.Location.X = -25;
                        break;
                    case SpawnPosition.Right:
                        cubeTransform.Location.X = -75;
                        break;
                }
                cubeTransform.Location.Y = 14.7f;
                cubeTransform.Location.Z = locationspawnCUbeZ;


                cubeTransform.Scale.X = 25.2f;
                cubeTransform.Scale.Y = 13.4f;
                cubeTransform.Scale.Z =100f;
                World.Instanciate(PhysicsCube, cubeTransform);
                //PowerUp
                {
                    Transform powerUPTransform = new Transform();
                    powerUPTransform.Location.X = cubeTransform.Location.X;
                    powerUPTransform.Location.Y = 32.5f;
                    powerUPTransform.Location.Z = cubeTransform.Location.Z-6.8f;

                    powerUPTransform.Scale = new Vector(3);
                    SpawnPowerUp(powerUPTransform);
                }
            }
        }

    }
}
