using Proof;
using System;

namespace OrbitBreak
{
    public class DeleteEntityScript : Entity
    {
        public float Lifetime = 5.0f;
        private float m_Timer = 0.0f;

        public Entity TargetEntity;
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
        {
            Lifetime = 1.5f;
        }
        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
        {
            
        }
        // OnPhysicsUpdate is called at a fixed timestep for physics-related logic
        void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
        {
        }

        protected virtual void OnPostUpdate(float ts) 
        {
            if (TargetEntity != null)
                Transform.Location = TargetEntity.Transform.Location;
            m_Timer += ts;
            if (m_Timer >= Lifetime)
            {
                World.DeleteEntity(this);
            }
        }

    }
}
