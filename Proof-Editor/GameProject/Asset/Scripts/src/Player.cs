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
        public bool Movement = true;
        private TransformComponent m_Transform;
        private RigidBodyComponent m_RigidBody;

        public float FowardForce = 200f;
        public float SideWayForce = 500f;
        void OnCreate()
        {
            m_Transform = GetComponent<TransformComponent>();
            m_RigidBody = GetComponent<RigidBodyComponent>();

        }

        void OnUpdate(float ts)
        {
            if (Movement == false)
                return;

            m_RigidBody.AddForce(new Vector(0, 0, FowardForce * ts));

            if (Input.IsKeyPressed(KeyBoardKey.D))
                m_RigidBody.AddForce(new Vector(-SideWayForce * ts, 0, 0),ForceMode.VelocityChange);

            if (Input.IsKeyPressed(KeyBoardKey.A))
                m_RigidBody.AddForce(new Vector(SideWayForce * ts, 0, 0), ForceMode.VelocityChange);

        }

        void OnCollisionEnter(Entity other)
        {
        }
    }
}
