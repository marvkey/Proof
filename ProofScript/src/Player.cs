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
        private TransformComponent m_TransformComponent;
        private RigidBodyComponent m_RigidiBody;
        public int Age = 17;
        public bool isCool = false;
        public uint Jump = 25;

        public float Sensitivity = 2.0f;
        int num
        {
            get
            {
                return num;
            }
            set
            {
                num = value;
            }
        }
        void OnCreate()
        {
            m_TransformComponent = GetComponent<TransformComponent>();
            m_RigidiBody = GetComponent<RigidBodyComponent>();
        }

        void OnUpdate(float ts)
        {
            if (Input.IsKeyPressed(KeyBoardKey.F))
            {
                Log.Trace($"Sensitivity {Sensitivity}");
            }
        }
    }
}
