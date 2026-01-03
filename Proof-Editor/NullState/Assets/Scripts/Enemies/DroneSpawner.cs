using System;
using System.Collections.Generic;
using Proof;
using Random = System.Random;

namespace NullState
{
    public class DroneSpawner : Entity
    {
        public int MinSpawnAmount = 2;
        public int MaxSpawnAmount = 5;
        public float SpawnRadius = 10;
        public Prefab Drone;
        public Entity SpawnPos;
        [SeperatorAttribute]
        public Entity Target;

        
        List<Entity> m_Drones = new List<Entity>();
        BoidFlockComponent m_BoidFlock;
        
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
        {
            m_BoidFlock = GetComponent<BoidFlockComponent>();
            if (m_BoidFlock == null)
            {
                Log.Info($"{Name} does not have boid flock component.");
                return;
            }
            Spawn();
        }
        
        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
        {
        }

        // OnPhysicsUpdate is called at a fixed timestep for physics-related logic
        void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
        {
        }

        public void Spawn()
        {
            if (m_BoidFlock == null)
            {
                Log.Info($"{Name} does not have boid flock component.");
                return;
            }
            if (Target == null)
            {
                Log.Error($"{Name}Target is null cant spawn drones");
                return;
            }

            if (Drone == null)
            {
                Log.Error($"{Name}Drone is null cant spawn drones");
                return;
            }
            int numSpawn = Proof.Random.Int(MinSpawnAmount, MaxSpawnAmount);
            
            Log.Info($"{Name} Spawning {numSpawn} spawn points.");
            for (int i = 0; i < numSpawn; i++)
            {
                Vector3 pos = Transform.Location + Proof.Random.InsideUnitSphere * SpawnRadius;
                Entity boid = World.Instantiate(Drone,pos);
                if (boid == null)
                {
                    Log.Error($"{Name} coudl not spawn");
                }

                boid.Transform.Forward = Proof.Random.InsideUnitSphere;
                
                m_BoidFlock.AddBoid(boid); 
                m_Drones.Add(boid);
                Drone drone = boid.GetScript<Drone>();
                if (drone != null)
                {
                    drone.FlockHolder = this;
                    drone.Target = m_BoidFlock.Target;
                }
            }
        }
    }
}