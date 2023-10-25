using System;
using System.Collections.Generic;
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
        public List<Entity> entity;
        public Entity[] EntityArray;
        private bool m_Movement = true;
        private TransformComponent m_Transform;
        private RigidBodyComponent m_RigidBody;
        private PlayerState m_State = PlayerState.Moving;
        public float FowardForce = 200f;
        public float SideWayForce = 500f;
        bool m_TouchingPlane = true;
        public float JumpSpeed = 3;
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

            m_RigidBody.AddForce(new Vector3(0, 0, FowardForce * ts));

            if (Input.IsKeyPressed(KeyBoardKey.D) || Input.IsKeyPressed(KeyBoardKey.RightArrow))
                m_RigidBody.AddForce(new Vector3(-SideWayForce * ts, 0, 0), ForceMode.VelocityChange);

            if (Input.IsKeyPressed(KeyBoardKey.A) || Input.IsKeyPressed(KeyBoardKey.LeftArrow))
                m_RigidBody.AddForce(new Vector3(SideWayForce * ts, 0, 0), ForceMode.VelocityChange);

            if (Input.IsKeyPressed(KeyBoardKey.Space) || Input.IsKeyPressed(KeyBoardKey.UpArrow))
                Jump();

            if(m_State == PlayerState.Jumpin)
            {
                m_RigidBody.SetLinearVelocity(new Vector3(m_RigidBody.GetLinearVelocity().X, m_RigidBody.GetLinearVelocity().Y-1, m_RigidBody.GetLinearVelocity().Z));
            }

            if(m_Transform.Location.Z % 100 == 0)
            {
                FowardForce += 30;
                SideWayForce += 15;
            }
        }
        
        public bool IsAlive()
        {
            return m_Movement != false;
        }

        private void Jump()
        {
            if (m_State == PlayerState.Jumpin)
                return;
            m_State = PlayerState.Jumpin;
            m_RigidBody.SetLinearVelocity(new Vector3(m_RigidBody.GetLinearVelocity().X, JumpSpeed, m_RigidBody.GetLinearVelocity().Z));
            m_TouchingPlane = false;
        }

        void OnCollisionEnter(Entity otherEntity)
        {
            if (otherEntity.Name == "Plane")
            {
                if (m_State == PlayerState.Jumpin)
                {
                    m_TouchingPlane = true;
                    m_State = PlayerState.Moving;
                    m_RigidBody.ClearTorque(ForceMode.Force);
                }
                return;
            }
        }
    }
}
