using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Proof;

namespace Game
{
    public class Enemy : Entity
    {
        public int Rating = 12;
        void OnUpdate(float ts)
        {
            if (Input.IsKeyPressed(KeyBoardKey.E))
            {
                Log.Trace($"Enemy rating {Rating}");
            }
        }
    }
}
