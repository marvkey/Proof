using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;

namespace Game
{
    class Explosive : PowerUp
    {
        public float ForceStrentght = 20.0f;
        protected override void ApplyPowerUp()
        {
            if (m_Owner == null || m_CollidedPlayer == null) return;

            m_CollidedPlayer.GetComponent<RigidBodyComponent>().AddForce(new Vector(0, ForceStrentght, 0), ForceMode.Impule);
            M_PowerupApplied = true;
        }

    }
}
