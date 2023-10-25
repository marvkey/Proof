using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Demos
{
    public enum StressLevel : int
    {
        Low = 0,
        Medium = 1,
        High = 100,
    }

    public enum TestType : int
    {
        BoxShape = 0,
        CubeRain,
    }

    class PhysicsCube : Entity
    {
        private int m_MaxCubeSpawn = 0;

        public StressLevel Level;
        public TestType Type;
        public Prefab SpawnObject;

        private int m_WallHeight = 10;
        private int m_WallLength = 30;
        private float m_Spacing = 2.5f;  // Adjust the spacing between walls.
        public void OnCreate()
        {
            // Create the four walls to form a square.
            //CreateWallZ(0);
            //CreateWallZ(60);
            //CreateWallX(0);
            //CreateWallX(60);

            switch (Level)
            {
                case StressLevel.Low:
                    m_MaxCubeSpawn = 100;
                        break; 
                case StressLevel.Medium:
                    m_MaxCubeSpawn = 100;
                        break;
                case StressLevel.High:
                    m_MaxCubeSpawn = 10000;
                    break;

            }
        }
        private int m_CubeRained = 0;
        protected void OnUpdate(float ts)
        {
            if(Type == TestType.CubeRain)
            {
                if (m_MaxCubeSpawn > m_CubeRained)
                    SpawnCubeRain(ts);
            }

        }

        private void SpawnCubeRain(float ts)
        {
            int numrain = Proof.Random.Int(0, 100);

            for(int i=0; i<numrain;i++)
            {
                if (m_MaxCubeSpawn < m_CubeRained)
                    return;
                Vector3 position = new Vector3(Proof.Random.Float(-100,100), Proof.Random.Float(50, 1000), Proof.Random.Float(-100, 100));
                World.Instanciate(SpawnObject, position);
                m_CubeRained++;
            }

        }

        private void CreateWallZ(float zPos =0)
        {
            for (int x = 0; x < m_WallLength; x++)
            {

                for (int y = 0; y < m_WallHeight; y++)
                {
                    Vector3 position = new Vector3(x *m_Spacing, y * m_Spacing, zPos);
                    World.Instanciate(SpawnObject, position);
                }
            }
        }

        private void CreateWallX(float xPos = 0)
        {
            for (int z = 0; z < m_WallLength; z++)
            {

                for (int y = 0; y < m_WallHeight; y++)
                {
                    Vector3 position = new Vector3(xPos , y * m_Spacing, z * m_Spacing);
                    World.Instanciate(SpawnObject, position);
                }
            }
        }
    }
}
