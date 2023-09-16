using Proof;
using ProofScriptCore.src.Proof.Math;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Game
{
    enum SpawnPosition
    {
        Left,
        Middle1,
        Middle2,
        Right
    }
    // 3 players world lenght 1500
    // 2 1000
    // 4 1800
    class mGameManager : Entity
    {
   
        public Prefab Obstacle;
        public Prefab DorrOpenerThing;
        public Prefab FinishLevel;
        //public Prefab WHiteDorrOpenerThing;
        //public Prefab WhteObstacle;

        public Prefab RampObstacle;
        public Prefab PhysicsCube;

        public Prefab Plane;
        public int WorldLenght = 1000;

        public Prefab PowerUp1;
        public Prefab PowerUp2;
        public Prefab PowerUp3;
        public Prefab PowerUp4;
        bool m_Restart = false;

        // current positon of player last
        public int CurrentPositon = 0;
       // public Prefab PowerUp4;
       // public Prefab PowerUp5;

        private List<Prefab> PowerUps = new List<Prefab>();

        public void OnCreate()  
        {
            if (Obstacle == null)
                return;
            int numPlayers = World.GetEntityWithType<MPlayer>().Length;

            if(numPlayers== 2)
            {
                WorldLenght = 1000;
            }
            else if(numPlayers == 3)
            {
                WorldLenght = 1500;

            }
            else if(numPlayers == 4)
            {
                WorldLenght = 1800;
            }
            Transform spawnTransform = new Transform();
            spawnTransform.Scale = new Vector(100, 1, WorldLenght);
            World.Instanciate(Plane, spawnTransform);

            PowerUps.Add(PowerUp1);
            PowerUps.Add(PowerUp2);
            PowerUps.Add(PowerUp3);
            PowerUps.Add(PowerUp4);
            for (int i = 80; i < WorldLenght; i+= Proof.Random.Int(50,100))
            {
                start:
                switch(Proof.Random.Int(1, 2)) 
                {
                    case 1:
                        {
                            if(i == 80)
                                goto start;
                            int originalWhenStart = i;
                            if (i > WorldLenght - 500)// gonna change to continue later
                            {
                                if (Proof.Random.Bool())
                                    continue;
                                else
                                    goto start;
                            }
                            int numBlocks = Proof.Random.Int(1, 2);
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
                            int originalWHneENd = i;
                            foreach (SpawnPosition pos in Enum.GetValues(typeof(SpawnPosition)))
                            {
                                if (spawnPositions.Contains(pos))
                                    continue;
                                int add = 0;
                                add += originalWhenStart;
                                for (int repeat =0; repeat < Proof.Random.Int(1,2); repeat++)
                                {
                                    add+= Proof.Random.Int(40, 70);
                                    switch (Proof.Random.Int(1, 3))
                                    {
                                        case 1:
                                            SpawnObstacle1(add, pos);
                                            break;
                                        case 2:
                                            SpawnDorrOpenThing(add, pos);
                                            break;
                                        case 3:
                                            {
                                                SpawnPowerUp(add, pos);
                                                    break;
                                            }
                                    }
                                }

                            }
                            i = originalWHneENd;
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
            spawnTransform = new Transform();
            spawnTransform.Location.Z = WorldLenght+100;
            spawnTransform.Location.Y = -50f;
            spawnTransform.Scale = new Vector(100, 0.5f, 100);

            World.Instanciate(FinishLevel, spawnTransform);

        }
        float m_RestartCounter = 10;
        void OnUpdate(float ts)
        {
            int dead = 0;
            List<MPlayer> currentPlayers = new List<MPlayer>();
            {
                MPlayer[] players = World.GetEntityWithType<MPlayer>();
                if (players.Length == 0) return;
                for (int i = 0; i < players.Length; i++)
                {
                    if (players[i].Finished == false)
                        currentPlayers.Add(players[i]);
                    if (players[i].Dead || players[i].Finished)
                        dead++;
                }
                if(dead == players.Length )
                {
                    m_Restart = true;
                    for (int i = 0; i < players.Length; i++)
                    {
                        players[i].Finished = true;
                    }
                }
               
            }
            if (m_Restart == true)
                m_RestartCounter -= ts;
            if(m_RestartCounter <= 0)
            {
                // (kinda workd)
                World.Restart();
            }
            if (currentPlayers.Count == 1)
            {
                //if (currentPlayers[0].Dead == true)
                    currentPlayers[0].StartFinishCountdown = true;
            }
            for (int i = 0; i < currentPlayers.Count - 1; i++)
            {
                for (int j = 0; j < currentPlayers.Count - 1 - i; j++)
                {
                    if (currentPlayers[j].GetComponent<TransformComponent>().Location.Z <
                        currentPlayers[j + 1].GetComponent<TransformComponent>().Location.Z)
                    {
                        MPlayer temp = currentPlayers[j];
                        currentPlayers[j] = currentPlayers[j + 1];
                        currentPlayers[j + 1] = temp;
                    }
                }
            }
            int pos = CurrentPositon;
            foreach (MPlayer player in currentPlayers)
            {
                pos++;
                player.Position = pos;
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

            //spawnTransform.Scale.Y = 1.25f;
            spawnTransform.Scale.Y = 4.5f;
            spawnTransform.Scale.Z = 1;
            spawnTransform.Scale.X = 25f;

            Entity ent = World.Instanciate(Obstacle, spawnTransform);
            //PowerUp
            {
                Transform powerUPTransform = new Transform();
                powerUPTransform.Location.X = spawnTransform.Location.X;
                powerUPTransform.Location.Y = spawnTransform.Location.Y +7.5f;
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

                SpawnPowerUp(powerUPTransform);
            }
        }
        private PowerUp SpawnPowerUp(int locationZ, SpawnPosition pos)
        {
            //return null;
            if (Proof.Random.Int(0, 3) == 3)
                return null;
            Transform spawnTransform = new Transform();

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
                    return null;
            }
            Prefab powerup = PowerUps[Proof.Random.Int(0, PowerUps.Count - 1)];
            spawnTransform.Scale = new Vector(2);
            spawnTransform.Location.Z = locationZ;
            spawnTransform.Location.Y = 4;
            Entity entity = World.Instanciate(powerup, spawnTransform);
            return entity.As<PowerUp>();
        }


        PowerUp SpawnPowerUp(Transform transform)
        {
            // return null;
            if (Proof.Random.Int(0, 3) == 3)
                return null;
            Prefab powerup = PowerUps[Proof.Random.Int(0,PowerUps.Count-1)];
             transform.Scale = new Vector(2);
             Entity entity = World.Instanciate(powerup, transform);
             return entity.As<PowerUp>();
        }
        //does not support middle
        private void SpawnRamp(int locationZ, SpawnPosition pos, int numBlocks =1)
        {
           
            Transform spawnTransform = new Transform();
            spawnTransform.Location.Z = locationZ-15f;// for adjustment
            spawnTransform.Location.Y = 14.1f;
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

            spawnTransform.Scale.X = 0.25f;
            spawnTransform.Scale.Y = 0.14f;
            spawnTransform.Scale.Z = 0.4f;
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
