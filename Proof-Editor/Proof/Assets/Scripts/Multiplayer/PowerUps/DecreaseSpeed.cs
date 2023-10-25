using Proof;

namespace Game
{
    class SpeedSlow : OpponentPowerUp
    {
        public float DecreaseSpeed = 100f;
        public float Duration = 5.0f;
        private float m_Timer;
        bool m_InitialApply = false;
        protected void OnCreate()
        {
            Image = new Texture(8483207687212316686);
        }
        protected override void ApplyPowerUp()
        {
            m_Timer += World.GetTimeStep();
            if (m_InitialApply  ==false)
            {
                InitialAddPowerUp();
                m_InitialApply=true;
            }
            if(m_Timer >= Duration)
            {
                M_PowerupApplied = true;

                m_Owner.FowardForce += DecreaseSpeed;
                m_Owner.SideWayForce += DecreaseSpeed / 3;
            }
        }

        void InitialAddPowerUp()
        {
            m_Owner.FowardForce -= DecreaseSpeed;
            m_Owner.SideWayForce -= DecreaseSpeed/3;
        }
    }
}
