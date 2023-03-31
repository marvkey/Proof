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
        private bool m_Movement = true;
        private TransformComponent m_Transform;
        private RigidBodyComponent m_RigidBody;

        public float FowardForce = 200f;
        public float SideWayForce = 500f;

        public void Destroy()
        {
            m_Movement = false;
            m_RigidBody.ClearForce(ForceMode.VelocityChange);
            m_RigidBody.ClearForce(ForceMode.Force);
            m_RigidBody.ClearTorque(ForceMode.Force);
        }
        
        void OnCreate()
        {
            m_Transform = GetComponent<TransformComponent>();
            m_RigidBody = GetComponent<RigidBodyComponent>();

        }

        void OnUpdate(float ts)
        {
            if (m_Movement == false)
                return;

            m_RigidBody.AddForce(new Vector(0, 0, FowardForce * ts));

            if (Input.IsKeyPressed(KeyBoardKey.D) || Input.IsKeyPressed(KeyBoardKey.RightArrow))
                m_RigidBody.AddForce(new Vector(-SideWayForce * ts, 0, 0),ForceMode.VelocityChange);

            if (Input.IsKeyPressed(KeyBoardKey.A) || Input.IsKeyPressed(KeyBoardKey.LeftArrow))
                m_RigidBody.AddForce(new Vector(SideWayForce * ts, 0, 0), ForceMode.VelocityChange);

        }
      
        public bool IsAlive()
        {
            return m_Movement != false;
        }
    }
}
