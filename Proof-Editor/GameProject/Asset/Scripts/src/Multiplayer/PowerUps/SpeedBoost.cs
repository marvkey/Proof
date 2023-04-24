using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace Game
{
    class SpeedBoost : PowerUp
    {
        public float IncrementSpeed = 100f;
        public float Duration = 5.0f;
        private float m_Timer;

        
        protected override void ApplyPowerUp()
        {
            m_Timer += World.GetTimeStep();

            if(m_Timer >= Duration)
            {
                M_PowerupApplied = true;

                m_Owner.FowardForce -= IncrementSpeed;
                m_Owner.SideWayForce -= IncrementSpeed / 2;
            }
        }
        void OnUpdate(float ts)
        {
            if (m_Throw == false && m_Owner != null)
            {
                GetComponent<TransformComponent>().Location = m_Owner.GetComponent<TransformComponent>().Location;
                GetComponent<TransformComponent>().Rotation = m_Owner.GetComponent<TransformComponent>().Rotation;
            }
            if (m_Throw)
                GetComponent<TransformComponent>().Translate(GetComponent<TransformComponent>().GetFowardVector() * Speed * ts);

            if (m_Owner != null)
                ApplyPowerUp();

            if (M_PowerupApplied == true)
            {
                World.DeleteEntity(this);
            }

        }

        void OnTriggerEnter(Entity other)
        {

        }

        public override void PickUp(MPlayer player)
        {
            base.PickUp(player);
        }

        void InitialAddPowerUp()
        {
            m_Owner.FowardForce += IncrementSpeed;
            m_Owner.SideWayForce += IncrementSpeed/2;
        }
    }
}
