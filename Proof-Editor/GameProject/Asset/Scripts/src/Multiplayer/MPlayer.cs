using Proof;
using ProofScriptCore.src.Proof.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Game
{

    public class MPlayer : Entity
    {
        public float FowardForce = 200f;
        public float SideWayForce = 500f;
        void OnCreate()
        {
            SetAction("NewAction", InputState.KeyPressed, Pressed);
        }

        void OnUpdate(float ts)
        {
        }
        void Pressed()
        {
            Log.Error("Pressed ");
        }
    }
}
