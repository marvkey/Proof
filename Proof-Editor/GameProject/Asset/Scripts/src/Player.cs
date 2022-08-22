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
        public float MovementSpeed = 2.0f;
        public float SlowSpeed = 3.0f;
        public float testSpeed = 4.5f;
        void OnCreate()
        {
            m_Transform = GetComponent<TransformComponent>();
        }

        void OnUpdate(float ts)
        {
            if (m_Transform == null)
                Log.Error("transform component null");
            else
                Log.Info($"{m_Transform.Location.X}");

            Vector currentLocation= new Vector(0.0f); //= m_Transform.Location;
            if (Input.IsKeyPressed(KeyBoardKey.DownArrow))
            {
                currentLocation.Y -= MovementSpeed;
            }
            if (Input.IsKeyPressed(KeyBoardKey.UpArrow))
            {
                currentLocation.Y += MovementSpeed;
            
            }
            
            if (Input.IsKeyPressed(KeyBoardKey.LeftArrow))
            {
                currentLocation.X -= MovementSpeed;
            }
            if (Input.IsKeyPressed(KeyBoardKey.RightArrow))
            {
                currentLocation.X += MovementSpeed;
            
            }

            //m_Transform.Location = currentLocation;
            if (Input.IsKeyPressed(KeyBoardKey.R))
            {
                Log.Trace($"Player movment speed {MovementSpeed}");
            }

        }
    }
}
