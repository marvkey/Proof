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
            m_RigidBody.AddForce(new Vector( 0,0, FowardForce * ts));

            if (Input.IsKeyPressed(KeyBoardKey.D))
                m_RigidBody.AddForce(new Vector(-SideWayForce * ts,0,0));

            if(Input.IsKeyPressed(KeyBoardKey.A))
                m_RigidBody.AddForce(new Vector(SideWayForce * ts,0,0));

        }
    }
}
