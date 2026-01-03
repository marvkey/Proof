using System;
using Proof;
using Random = System.Random;

namespace NullState
{
    public class TestBoid : Entity
    {
        public Prefab Prefab;
        BoidFlockComponent m_BoidFlock;
        
        public float spawnRadius = 10;
        public int spawnCount = 10;
        void OnCreate()
        {
            m_BoidFlock = GetComponent<BoidFlockComponent>();
            if (m_BoidFlock == null)
            {
                Log.Info($"{Name} does not have boid flock component.");
                return;
            }

            if (Prefab == null)
            {
                Log.Info($"{Name} does not have prefab component.");
                return;
            }
            
            
            for (int i = 0; i < spawnCount; i++) {
                Vector3 pos = Transform.Location + Proof.Random.InsideUnitSphere * spawnRadius;
                Entity boid = World.Instantiate(Prefab,pos);

                boid.Transform.Forward = Proof.Random.InsideUnitSphere;
                
                m_BoidFlock.AddBoid(boid); 
            }
        }

        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
        {
        }

        // OnPhysicsUpdate is called at a fixed timestep for physics-related logic
        void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
        {
        }
        
        
        
    }
}