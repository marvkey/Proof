using System;
using Proof;

namespace NullState
{
    public class EntropyCompnent : Entity
    {
        public float Entropy = 0.0f;
        public float MaxEntropy = 100f;

        public float DecayRate = 1f; // <- always reduces entropy per second
        public float EntropyPercent
        {
            get { return Entropy / MaxEntropy; }
        }
        //public float IdleDecay = 2.5f;
        //public float IdleWaitTime = 5.0f; // if idle more than 5 seconds
        [SeperatorAttribute]
        public bool SpeedEntropy = true;
        public float SpeedThreshold = 18.0f;
        [ToolTip("This is also multiplied by teh speed")]
        public float SpeedGainMultiplier = 0.05f;


        private Vector3 m_LastPosition;

        void OnCreate()
        {
            m_LastPosition = Transform.Location;
        }

        void OnUpdate(float deltaTime)
        {
            Entropy -= DecayRate * deltaTime;
            HandleMovmentDecay(deltaTime);

            Entropy = Mathf.Clamp(Entropy, 0f, MaxEntropy);

        }

        void HandleMovmentDecay(float dt)
        {
            // Calculate speed
            float distance = (Transform.Location - m_LastPosition).Length;
            float speed = distance / dt;
               
            if (speed > SpeedThreshold)
                Entropy += speed * SpeedGainMultiplier * dt;

            // Clamp between 0 and MaxEntropy

            m_LastPosition = Transform.Location;

        }
    }
}
   