using Proof;
using System;

namespace Game
{
     class Shake : OpponentPowerUp
    {
        public float ShakeDuration = 3.0f;
        bool m_InitialPowerUp = false;
        float m_PlayerInitialPosY;
        float m_PlayerInitialPosX;
        private float m_Timer = 0;
        protected void OnCreate()
        {
            Image = new Texture(7247838432740145550);
            GetComponent<TransformComponent>().Scale = new Vector3(100);
            GetComponent<TransformComponent>().Location = new Vector3(GetComponent<TransformComponent>().Location.X, GetComponent<TransformComponent>().Location.Y + 8, GetComponent<TransformComponent>().Location.Z);
        }
        protected override void ApplyPowerUp()
        {
            if (m_Owner == null || m_CollidedPlayer == null) return;
            if (m_InitialPowerUp == false)
            {
                m_InitialPowerUp = true;
                m_PlayerInitialPosY = m_CollidedPlayer.GetComponent<TransformComponent>().Location.Y;
                m_PlayerInitialPosX = m_CollidedPlayer.GetComponent<TransformComponent>().Location.X;
                m_CollidedPlayer.GetComponent<ParticleSystemComponent>().GetParticle(2).Play();
            }

            if (m_PlayerInitialPosY < m_CollidedPlayer.GetComponent<TransformComponent>().Location.Y - 3 || m_Timer != 0)
                ApplyShake();
            else
            {
                m_CollidedPlayer.GetComponent<TransformComponent>().Translate(new Vector3(0, 0.1f, 0));
                m_CollidedPlayer.GetComponent<RigidBodyComponent>().Gravity = false;
                m_CollidedPlayer.DisableFowardForce = true;

                foreach (int i in Enum.GetValues(typeof(ForceMode)))
                {
                    m_CollidedPlayer.GetComponent<RigidBodyComponent>().ClearForce((ForceMode)i);
                    m_CollidedPlayer.GetComponent<RigidBodyComponent>().ClearTorque((ForceMode)i);
                }
            }
        }

        bool add = true;
        Vector3 pos = new Vector3(0, 30, 0);
        void ApplyShake()
        {
            if(add == true)
            {
                add = false;
                Vector3 val = m_CollidedPlayer.GetComponent<TransformComponent>().Location;
                val.X += Proof.Random.Float(1, 5);
                val.Y += Proof.Random.Float(0.5f, 2.5f);
                m_CollidedPlayer.GetComponent<TransformComponent>().Location = val;
            }
            else
            {
                Vector3 val = m_CollidedPlayer.GetComponent<TransformComponent>().Location;
                val.X -= Proof.Random.Float(1, 5);
                val.Y -= Proof.Random.Float(0.5f, 2.5f);
                m_CollidedPlayer.GetComponent<TransformComponent>().Location = val;
                add = true;
            }
            m_CollidedPlayer.GetComponent<TransformComponent>().Rotation += 3;
            foreach (int i in Enum.GetValues(typeof(ForceMode)))
            {
                m_CollidedPlayer.GetComponent<RigidBodyComponent>().ClearForce((ForceMode)i);
                m_CollidedPlayer.GetComponent<RigidBodyComponent>().ClearTorque((ForceMode)i);
            }
            m_Timer += World.GetTimeStep();
            m_CollidedPlayer.GetComponent<RigidBodyComponent>().SetLinearVelocity(new Vector3(0));
            m_CollidedPlayer.GetComponent<RigidBodyComponent>().SetAngularVelocity(new Vector3(0));

            if (m_Timer >= ShakeDuration)
            {
                Log.Info("Done shaking");
                m_CollidedPlayer.GetComponent<TransformComponent>().Rotation = new Vector3(0);
               // m_CollidedPlayer.GetComponent<TransformComponent>().Rotation = new Vector3(0);
                m_CollidedPlayer.DisableFowardForce = false;
                m_CollidedPlayer.GetComponent<RigidBodyComponent>().Gravity = true;

                Vector3 val = m_CollidedPlayer.GetComponent<TransformComponent>().Location;
                val.X = m_PlayerInitialPosX;
                val.Y = m_PlayerInitialPosY;
                m_CollidedPlayer.GetComponent<TransformComponent>().Location = val;
                M_PowerupApplied = true;
            }
        }
    }
}
