
using System;
using System.Reflection.Emit;
using Proof;

namespace AltoDemo
{
	public class SceneController : Entity
	{
		public Entity[] Spikes;
        public Entity Player;
		public Entity Ground;
        private int m_FurthestSpikeIndex;
        private float m_AngleOfGround;

        private TextComponent m_Tc;
        Transform m_PlayerTransform;

        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{
            m_Tc = GetComponent<TextComponent>();
            m_FurthestSpikeIndex = 4;
            m_AngleOfGround = Ground.GetComponent<RigidBodyComponent>().Rotation.X;

        }

        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
		{
            if (Player != null)
            {
                m_PlayerTransform = Player.Transform.WorldTransform;
                MoveSpikes();
            }
        }

        void MoveSpikes()
        {
            for (int i = 0; i < Spikes.Length; i++)
            {
                if (Spikes[i] == null)
                    continue;
                if (Spikes[i].Location.Z > Player.Location.Z && Mathf.Abs(Spikes[i].Location.Z - Player.Location.Z) > 26.0f)
                {
                    float currentFurthestSpikeZ = Spikes[m_FurthestSpikeIndex].Location.Z;
                    float offset = Proof.Random.Float(-7.0f,-13.0f);
                    float newZ = currentFurthestSpikeZ + offset;

                    float newY = Mathf.Tan( m_AngleOfGround) * newZ; // Trigonometric way of figuring out the new Y location Tan(theta) = Opp / Adj

                    m_FurthestSpikeIndex = i;
                    Spikes[i].Location = new Vector3(Spikes[i].Location.X, -newY, newZ);
                }
            }
        }
        
	}
}
