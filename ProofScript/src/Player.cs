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
        void OnCreate()
        {
            m_TransformComponent = GetComponent<TransformComponent>();
            m_RigidiBody = GetComponent<RigidBodyComponent>();
        }

        void OnUpdate(float ts)
        {
            if (Input.IsKeyPressed(KeyBoardKey.F))
            {
                m_RigidiBody.AddForce(new Vector(0, 1, 10));
                Log.Trace("Added force to entity");
            }
        }
    }
}
