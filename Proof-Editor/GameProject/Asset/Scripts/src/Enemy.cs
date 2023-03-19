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
            Player obj = other.As<Player>();
            if (obj != null)
            {
                obj.Movement = false;
                Log.Info($"{GetComponent<TagComponent>().Tag} Removed {obj.GetComponent<TagComponent>().Tag} Movment");

            }
        }
    }
}

