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
        void OnCreate()
        {
            m_TransformComponent = GetComponent<TransformComponent>();
        }

        void OnUpdate(float ts)
        {
            m_TransformComponent.Location = new Vector(Proof.Random.Float(0, 10),1,1);
            Console.WriteLine($"m_TransformComponent.Location{m_TransformComponent.Location.X}");
        }

    }
}
