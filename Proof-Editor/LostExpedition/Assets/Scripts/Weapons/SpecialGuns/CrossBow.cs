using System;
using Proof;

namespace LostExpedition
{
    public class Arrow : Entity
    {
        void OnCreate()
        {
            TriggerEnterEvent += OnTrigger;
        }
        void OnPhysicsUpdate(float dt)
        {
           
        }

        void OnTrigger(Entity e)
        {
            if(e!= this && e != GetParent())
            {
               e.GetParent()?.GetScript<CrossBow>()?.ArrowHit(e);
            }
        }
    }
    public class CrossBow : Gun
    {
        // local pos where arrow shoudl spaawn from 
        public Entity SpawnPos;
        public Prefab ArrowPrefab;
        public float ArrowSpeed = 60f;     // Speed of arrow

        Entity m_CurrentPlayer = null;
       // protected override Entity Shoot(Entity playerEntity,TransformComponent playerTransform, TransformComponent cameraTransform) 
       // {
       //     m_CurrentPlayer = playerEntity;
       //
       //     ShootArrow(playerEntity, playerTransform, cameraTransform);
       //     return null;
       // }

        
        public void ArrowHit(Entity e)
        {
            if (e.GetScript<HealthComponent>() != null)
            {
                e.GetScript<HealthComponent>().TakeDamage(m_CurrentPlayer,Damage);
            }
        }

        void ShootArrow(Entity playerEntity, TransformComponent playerTransform, TransformComponent cameraTransform)
        {
            /*
            // 1. Instantiate the arrow
            Entity arrow = World.Instantiate(ArrowPrefab, SpawnPos.Transform.WorldTransform);


            // 3. Launch arrow toward crosshair using camera forward
            Vector3 direction = cameraTransform.Forward.Normalized;
            RigidBodyComponent rb = arrow.GetComponent<RigidBodyComponent>();

            if (rb != null)
            {
                rb.Velocity = direction * ArrowSpeed;
            }

            // 4. Optional: Make arrow face direction of travel
            //arrowTransform.Rotation = Quaternion.LookRotation(direction);

            arrow.Transform.RotationQuat = Quaternion.loook(direction);
            */
        }

    }
}
