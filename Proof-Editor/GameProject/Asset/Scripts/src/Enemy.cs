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
        
        void OnCreate()
        {
        }

        void OnUpdate(float ts)
        {

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
    }
}

