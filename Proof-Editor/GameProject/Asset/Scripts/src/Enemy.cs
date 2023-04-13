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
        void OnCreate()
        {
            MainPlayer = World.TryFindEntityByTag("Player");
        }

        void OnUpdate(float ts)
        {
            if (MainPlayer == null) return;

            Log.Info("unoadjsfdnadfnsajndk");
            if (MainPlayer.GetComponent<TransformComponent>().Location.Z < this.GetComponent<TransformComponent>().Location.Z)
                World.DeleteEntity(this);
        }

        void OnCollisionEnter(Entity other)
        {
            Player player = other.As<Player>();
            if (player != null)
            {
                player.Destroy();
                Log.Info($"{GetComponent<TagComponent>().Tag} Removed {player.GetComponent<TagComponent>().Tag} Movment");

            }
        }

        void OnDestroy()
        {
            Log.Info("destoryed");
        }
    }
}

