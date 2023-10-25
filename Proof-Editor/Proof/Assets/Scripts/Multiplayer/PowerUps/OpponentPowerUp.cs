using Proof;

namespace Game
{
    class OpponentPowerUp : PowerUp
    {
        protected void OnUpdate(float ts)
        {
            if (m_Throw == false && m_Owner != null)
            {
                GetComponent<TransformComponent>().Location = m_Owner.GetComponent<TransformComponent>().Location;
                GetComponent<TransformComponent>().Rotation = new Vector3(GetComponent<TransformComponent>().Rotation.X, GetComponent<TransformComponent>().Location.Y, m_Owner.GetComponent<TransformComponent>().Rotation.Z);
            }
            if (m_Throw)
            {
                GetComponent<TransformComponent>().Translate(GetComponent<TransformComponent>().GetFowardVector() * Speed * ts);
                m_Tmer -= ts;
            }
            if(m_Tmer < 0 || GetComponent<TransformComponent>().Location.Z >= 2500)
            {
                World.DeleteEntity(this);
            }
            if (m_CollidedPlayer != null && M_PowerupApplied == false && m_Throw == false)
                ApplyPowerUp();

            if (m_CollidedPlayer != null && M_PowerupApplied == true && m_Throw == true)
            {
                World.DeleteEntity(this);
            }
        }
        // did not hit anyting
        private float m_Tmer = 4;
        public override void Throw()
        {
            base.Throw();
            GetComponent<MeshComponent>().Visible = true;
        }

    }
}
