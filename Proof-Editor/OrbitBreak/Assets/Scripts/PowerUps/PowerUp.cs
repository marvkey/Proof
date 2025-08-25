using Proof;
using System;

namespace OrbitBreak
{
    public class PowerUp :Entity
    {
        public enum State
        {
            None = 0,
            Shield = 1,
            Gun = 2,
            Speed = 3,

        }

        public float StartDuration = 5.0f;

        private State m_State = State.None;
        private float m_DurationTimer = 0.0f;

        public bool IsActive
        {
            get { return m_State != State.None; }
        }

        public float DurationTimer
        {
            get { return m_DurationTimer; }
            set { m_DurationTimer = value; }
        }

        public State CurrentState
        {
            get { return m_State; }
        }



        public float SpeedScale = 2.0f;
        public void SetState(State state)
        {
            m_State = state;
            m_DurationTimer = StartDuration;
        }
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
        {

        }

        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
        {
          
        }

        // OnPhysicsUpdate is called at a fixed timestep for physics-related logic
        void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
        {
        }

        

    }
}
