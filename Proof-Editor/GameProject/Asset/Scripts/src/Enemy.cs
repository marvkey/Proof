using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace Game
{

    public class Enemy : Entity
    {
        public Entity MainPlayer;

        float m_Timer = 0.5f;
        void OnCreate()
        {
            MainPlayer = World.TryFindEntityByTag("Player");
        }

        void OnUpdate(float ts)
        {
            if (MainPlayer == null) return;

            if (MainPlayer.GetComponent<TransformComponent>().Location.Z > this.GetComponent<TransformComponent>().Location.Z)
                m_Timer -= ts;
            if (m_Timer <= 0)
                World.DeleteEntity(this);
        }

        void OnCollisionEnter(Entity other)
        {
            Player player = other.As<Player>();
            if (player != null)
            {
                player.Destroy();
            }
        }
      
    }
}

