using Proof;

namespace Game
{
    class SpeedBoost : PlayerPowerUp
    {
        public float IncrementSpeed = 600f;
        public float Duration = 5.0f;
        private float m_Timer;
        bool m_HasRUnTrnasform = false;
        bool m_InitialPowerUp = false;
        public override float GetCountDown()
        {
            return m_Timer;
        }
        protected void OnCreate()
        {
            Image = new ImageAsset(10460736942826054302);
        }

        protected override void ApplyPowerUp()
        {
            m_Timer -= World.GetTimeStep();
            if(m_InitialPowerUp == false)
            {
                m_Timer = Duration;
                m_Owner.m_ParticleSystem.GetParticle(3).Play();
                InitialAddPowerUp();
                m_InitialPowerUp = true;
            }
            if(m_Timer <= 0.0f)
            {
                M_PowerupApplied = true;

                m_Owner.FowardForce -= IncrementSpeed;
                m_Owner.SideWayForce -= IncrementSpeed / 3;
                m_Owner.m_ParticleSystem.GetParticle(3).End();
            }
        }

        protected void OnUpdate(float ts)
        {
            base.OnUpdate(ts);
      
            if (m_Owner == null && m_Throw == false && m_HasRUnTrnasform == false)
            {
                GetComponent<TransformComponent>().Rotation = new Vector(0, 0, 180);
                GetComponent<TransformComponent>().Location = new Vector(GetComponent<TransformComponent>().Location.X, GetComponent<TransformComponent>().Location.Y + 3,
                    GetComponent<TransformComponent>().Location.Z);
                m_HasRUnTrnasform = true;

            }
            if (m_Throw == false && m_Owner != null)
            {
                GetComponent<TransformComponent>().Location = m_Owner.GetComponent<TransformComponent>().Location;
                GetComponent<TransformComponent>().Rotation = new Vector(m_Owner.GetComponent<TransformComponent>().Rotation.X, m_Owner.GetComponent<TransformComponent>().Rotation.Y,
                    180);
            }
            if (M_PowerupApplied == true)
            {
                m_Owner.RemoveItem(this);
                World.DeleteEntity(this);
            }
        }
        void InitialAddPowerUp()
        {
            m_Owner.FowardForce += IncrementSpeed;
            m_Owner.SideWayForce += IncrementSpeed/3;
        }
    }
}
