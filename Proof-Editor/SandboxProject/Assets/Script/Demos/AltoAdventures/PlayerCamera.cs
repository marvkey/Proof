
using System;
using Proof;

namespace AltoDemo
{
	public class PlayerCamera : Entity
	{
        public Entity Player;
		public Vector4 vectro;
		
		public float testValue =10;
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{ 
		}

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{

            if (Player != null)
			{
                TransformComponent transform = GetComponent<TransformComponent>();
				transform.Location = new Vector3(transform.Location.X, Player.GetComponent<TransformComponent>().Location.Y + 10.0f, Player.GetComponent<TransformComponent>().Location.Z - 0.7f);
			}
        }

	}
}
