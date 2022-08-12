using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Proof;
namespace Game
{
    public class Player : Entity
    {
        void OnCreate()
        {

        }

        void OnUpdate(float ts)
        {
            Console.WriteLine(ts);
        }


    }
}
