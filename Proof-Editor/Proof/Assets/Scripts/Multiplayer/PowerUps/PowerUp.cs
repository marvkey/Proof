using Proof;

namespace Game
{
    
    public class PowerUp : Entity
    {
        
        protected virtual void ApplyPowerUp()
        {

        }
       
        public Texture Image;

        public float Speed = 400.0f;
        protected MPlayer m_Owner = null;

        protected bool m_Throw = false;
        protected MPlayer m_CollidedPlayer = null;

        protected bool M_PowerupApplied = false;
        
      
        public bool HasBeenThrown()
        {
            return m_Throw;
        }
      
        public virtual void Throw()
        {
            m_Throw = true;
        }

        public void PickUp(MPlayer player)
        {
            if (player == null) return;
            m_Owner = player;
            this.GetComponent<MeshComponent>().Visible = false;
        }

        public void Drop()
        {
            m_Owner = null;
        }
        public bool HasOwner()
        {
            return m_Owner != null;
        }
        void OnTriggerEnter(Entity other)
        {

            MPlayer player = other.As<MPlayer>();
            if (player != m_Owner && m_Owner != null && m_Throw == true)
            {
                m_Throw = false;
                m_CollidedPlayer = player;
            }

            if(m_Throw == true && player == null)
            {
                World.DeleteEntity(this);
            }
        }

    }
}
