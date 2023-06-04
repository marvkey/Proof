using Proof;

namespace Game
{
    class PlayerPowerUp : PowerUp
    {
        public virtual float GetCountDown()
        {
            return 0.0f;
        }
        public override void Throw()
        {
            base.Throw();
            GetComponent<MeshComponent>().Visible = false;
        }


        protected void OnUpdate(float ts)
        {
            if (m_Owner == null)
                return;
            
            // m_throw is acting as the power up has been applied 
            if (m_Owner != null && m_Throw)
                ApplyPowerUp();
        }
    }
}
