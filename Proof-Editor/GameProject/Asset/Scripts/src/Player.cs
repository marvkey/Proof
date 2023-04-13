using System;
using System.Collections.Generic;
using System.Diagnostics.Tracing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Proof;

namespace Game
{
    enum PlayerState:UInt32
    {
        None =0,
        Moving =1,
        Jumpin =2
        
    }
    public class Player : Entity
    {
        private bool m_Movement = true;
        private TransformComponent m_Transform;
        private RigidBodyComponent m_RigidBody;
        private PlayerState m_State = PlayerState.Moving;
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
                m_RigidBody.AddForce(new Vector(-SideWayForce * ts, 0, 0), ForceMode.VelocityChange);

            if (Input.IsKeyPressed(KeyBoardKey.A) || Input.IsKeyPressed(KeyBoardKey.LeftArrow))
                m_RigidBody.AddForce(new Vector(SideWayForce * ts, 0, 0), ForceMode.VelocityChange);

            if (Input.IsKeyPressed(KeyBoardKey.Space) || Input.IsKeyPressed(KeyBoardKey.UpArrow))
                Jump();

            if(m_State == PlayerState.Jumpin)
                m_RigidBody.AddForce(new Vector(0,-1,0),ForceMode.VelocityChange);
        }
        
        public bool IsAlive()
        {
            return m_Movement != false;
        }

        private void Jump()
        {
            if (m_State == PlayerState.Jumpin)
                return;
            m_RigidBody.AddForce(new Vector(0, 20,0), ForceMode.VelocityChange);
            m_State = PlayerState.Jumpin;
            //m_RigidBody.SetLinearVelocity(new Vector(0, 10, m_RigidBody.GetLinearVelocity().Z));
        }

        void OnCollisionEnter(Entity leav)
        {
            if (!(leav.Name == "Plane"))
                return;
            m_State = PlayerState.Moving;
            Log.Info("Baack moving");
        }
    }
}
