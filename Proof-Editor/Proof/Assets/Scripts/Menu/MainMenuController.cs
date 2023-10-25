using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;

namespace Game
{
    public class MainMenuController : Entity
    {
        protected void OnUpdate(float ts)
        {
            if (Input.IsKeyClicked(KeyBoardKey.P))
            {
                World.OpenWorld(13308300816728474787);
            }

            if (Input.IsKeyClicked(KeyBoardKey.Q))
            {
                
            }
        }
    }
}
