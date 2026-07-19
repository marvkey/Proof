using System;
using System.Collections.Generic;
using Proof;

namespace SandboxProject
{
    public class PointLightStressTest : Entity
    {
        public int LightCount = 10000;

        public Vector3 SpawnBoxCenter = new Vector3(0.0f, 5.0f, 0.0f);
        public Vector3 SpawnBoxSize = new Vector3(50.0f, 20.0f, 50.0f);

        public float MinSpeed = 1.0f;
        public float MaxSpeed = 6.0f;

        public float LightRadius = 5.0f;
        public float LightIntensity = 2.0f;

        public bool SpawnOnStart = false;

        private bool m_Spawned = false;
        private bool m_Paused = false;

        private List<Entity> m_LightEntities = new List<Entity>();
        private List<TransformComponent> m_LightTransforms = new List<TransformComponent>();
        private List<Vector3> m_Velocities = new List<Vector3>();

        private System.Random m_Random = new System.Random();

        protected override void OnCreate()
        {
            m_LightEntities = new List<Entity>(LightCount);
            m_LightTransforms = new List<TransformComponent>(LightCount);
            m_Velocities = new List<Vector3>(LightCount);

            if (SpawnOnStart)
                SpawnLights();
        }

        protected override void OnUpdate(float deltaTime)
        {
            if (!m_Spawned && Input.IsKeyPressed(KeyBoardKey.L))
            {
                SpawnLights();
            }                                

            if (Input.IsKeyPressed(KeyBoardKey.P))
            {
                m_Paused = !m_Paused;

                if (m_Paused)
                    Console.WriteLine("Point light movement paused.");
                else
                    Console.WriteLine("Point light movement playing.");
            }

            if (m_Spawned && !m_Paused)
            {
                UpdateLights(deltaTime);
            }
        }

        private void SpawnLights()
        {
            if (m_Spawned)
                return;

            m_Spawned = true;

            Vector3 halfSize = SpawnBoxSize * 0.5f;

            for (int i = 0; i < LightCount; i++)
            {
                Vector3 position = new Vector3(
                    RandomRange(SpawnBoxCenter.x - halfSize.x, SpawnBoxCenter.x + halfSize.x),
                    RandomRange(SpawnBoxCenter.y - halfSize.y, SpawnBoxCenter.y + halfSize.y),
                    RandomRange(SpawnBoxCenter.z - halfSize.z, SpawnBoxCenter.z + halfSize.z)
                );

                Entity lightEntity = World.CreateEntity("Stress Test Point Light");

                TransformComponent transform = lightEntity.GetComponent<TransformComponent>();
                transform.Location = position;

                Proof.PointLightComponent pointLight = lightEntity.AddComponent<Proof.PointLightComponent>();
                pointLight.Color = RandomColor();
                pointLight.Intensity = LightIntensity;
                pointLight.Radius = LightRadius;

                Vector3 direction = RandomDirection();
                float speed = RandomRange(MinSpeed, MaxSpeed);

                m_LightEntities.Add(lightEntity);
                m_LightTransforms.Add(transform);
                m_Velocities.Add(direction * speed);
            }

            Console.WriteLine("Spawned " + LightCount + " moving point lights.");
        }

        private void UpdateLights(float deltaTime)
        {
            Vector3 halfSize = SpawnBoxSize * 0.5f;

            Vector3 minBounds = SpawnBoxCenter - halfSize;
            Vector3 maxBounds = SpawnBoxCenter + halfSize;

            for (int i = 0; i < m_LightEntities.Count; i++)
            {
                Entity lightEntity = m_LightEntities[i];

                if (!lightEntity)
                    continue;

                TransformComponent transform = m_LightTransforms[i];

                Vector3 position = transform.Location;
                Vector3 velocity = m_Velocities[i];

                position += velocity * deltaTime;

                if (position.x < minBounds.x || position.x > maxBounds.x)
                {
                    velocity.x *= -1.0f;
                    position.x = Clamp(position.x, minBounds.x, maxBounds.x);
                }

                if (position.y < minBounds.y || position.y > maxBounds.y)
                {
                    velocity.y *= -1.0f;
                    position.y = Clamp(position.y, minBounds.y, maxBounds.y);
                }

                if (position.z < minBounds.z || position.z > maxBounds.z)
                {
                    velocity.z *= -1.0f;
                    position.z = Clamp(position.z, minBounds.z, maxBounds.z);
                }

                transform.Location = position;
                m_Velocities[i] = velocity;
            }
        }

        private float RandomRange(float min, float max)
        {
            return min + (float)m_Random.NextDouble() * (max - min);
        }

        private Vector3 RandomDirection()
        {
            Vector3 direction = new Vector3(
                RandomRange(-1.0f, 1.0f),
                RandomRange(-1.0f, 1.0f),
                RandomRange(-1.0f, 1.0f)
            );

            if (direction.LengthSqr < 0.0001f)
                return new Vector3(1.0f, 0.0f, 0.0f);

            return direction.Normalized;
        }

        private Vector3 RandomColor()
        {
            return new Vector3(
                RandomRange(0.2f, 1.0f),
                RandomRange(0.2f, 1.0f),
                RandomRange(0.2f, 1.0f)
            );
        }

        private float Clamp(float value, float min, float max)
        {
            if (value < min)
                return min;

            if (value > max)
                return max;

            return value;
        }
    }
}