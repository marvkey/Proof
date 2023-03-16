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
            Log.Info("On create");
            Log.Info($"Location {m_Transform.Location.X}");

        }

        void OnUpdate(float ts)
        {
            if (m_Transform == null)
                Log.Error("transform component null");

            Vector currentLocation = m_Transform.Location;
            if (Input.IsKeyPressed(KeyBoardKey.DownArrow))
            {
                Log.Trace("Movement");
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
            m_Transform.Location = currentLocation;
            if (Input.IsKeyPressed(KeyBoardKey.R))
            {
                Log.Trace($"Player movment speed {MovementSpeed}");
            }

        }
    }
}
