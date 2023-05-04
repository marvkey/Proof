using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Game
{
    class PowerUp : Entity
    {
        protected virtual void ApplyPowerUp()
        {

        }
        public ImageAsset Image;

        public float Speed = 3.0f;
        protected MPlayer m_Owner = null;

        protected bool m_Throw = false;
        protected MPlayer m_CollidedPlayer = null;

        protected bool M_PowerupApplied = false;
        void OnCreate()
        {

        }
       void OnUpdate(float ts)
        {
            if(m_Throw == false && m_Owner != null)
            {
                GetComponent<TransformComponent>().Location = m_Owner.GetComponent<TransformComponent>().Location;
                GetComponent<TransformComponent>().Rotation = m_Owner.GetComponent<TransformComponent>().Rotation;
            }
            if(m_Throw)
                GetComponent<TransformComponent>().Translate(GetComponent<TransformComponent>().GetFowardVector() * Speed * ts);

            if(m_CollidedPlayer != null && M_PowerupApplied == false)
                ApplyPowerUp();

            if(m_CollidedPlayer != null && M_PowerupApplied == true)
            {
                World.DeleteEntity(this);
            }

        }

      
        public void Throw()
        {
            m_Throw = true;
        }

        virtual public void PickUp(MPlayer player)
        {
            m_Owner = player;
            GetComponent<MeshComponent>().Visible = false;
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
            if (player != m_Owner && m_Owner != null)
            {
                m_Throw = false;
                m_CollidedPlayer = player;
            }
        }

    }
}
