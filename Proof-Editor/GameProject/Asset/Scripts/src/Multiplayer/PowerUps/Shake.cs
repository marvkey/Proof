using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace Game
{
     class Shake : PowerUp
    {
        public float ShakeDuration = 3.0f;
        bool m_InitialPowerUp = false;
        float m_PlayerInitialPosY;
        private float m_Timer = 0;
        protected override void ApplyPowerUp()
        {
            if (m_Owner == null || m_CollidedPlayer == null) return;
            if (!m_InitialPowerUp)
            {
                m_InitialPowerUp = true;
                m_PlayerInitialPosY = m_CollidedPlayer.GetComponent<TransformComponent>().Location.Y;
            }

            if (m_PlayerInitialPosY < m_CollidedPlayer.GetComponent<TransformComponent>().Location.Y - 3)
                ApplyShake();
            else
            {
                m_CollidedPlayer.GetComponent<TransformComponent>().Translate(new Vector(0, 0.1f, 0));
                m_CollidedPlayer.GetComponent<RigidBodyComponent>().Gravity = false;
                m_CollidedPlayer.DisableFowardForce = true;

                foreach (int i in Enum.GetValues(typeof(ForceMode)))
                {
                    m_CollidedPlayer.GetComponent<RigidBodyComponent>().ClearForce((ForceMode)i);
                    m_CollidedPlayer.GetComponent<RigidBodyComponent>().ClearTorque((ForceMode)i);
                }
            }
        }

        void ApplyShake()
        {
            m_CollidedPlayer.GetComponent<TransformComponent>().Rotation += 3;
            foreach (int i in Enum.GetValues(typeof(ForceMode)))
            {
                m_CollidedPlayer.GetComponent<RigidBodyComponent>().ClearForce((ForceMode)i);
                m_CollidedPlayer.GetComponent<RigidBodyComponent>().ClearTorque((ForceMode)i);
            }
            m_Timer += World.GetTimeStep();
            m_CollidedPlayer.GetComponent<RigidBodyComponent>().SetLinearVelocity(new Vector(0));
            m_CollidedPlayer.GetComponent<RigidBodyComponent>().SetAngularVelocity(new Vector(0));

            if (m_Timer >= ShakeDuration)
            {
                Log.Info("Done shaking");
                m_CollidedPlayer.GetComponent<TransformComponent>().Rotation = new Vector(0);
                m_CollidedPlayer.GetComponent<TransformComponent>().Rotation = new Vector(0);
                m_CollidedPlayer.DisableFowardForce = false;
                m_CollidedPlayer.GetComponent<RigidBodyComponent>().Gravity = true;

                M_PowerupApplied = true;
            }
        }
    }
}
