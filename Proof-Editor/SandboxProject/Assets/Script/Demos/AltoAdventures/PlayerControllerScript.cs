
using System;
using Proof;

namespace AltoDemo
{
	public class PlayerControllerScript : Entity
	{
        private RigidBodyComponent m_Rb;
        private bool m_ShouldJump;
        public float JumpForce = 16.0f;
        public float MaxRayLength = 0.01f;
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{
            CollisionEnterEvent += OnCollisionBegin;

            m_Rb = GetComponent<RigidBodyComponent>();
            m_Rb.MaxLinearVelocity = 18.0f;// Setting a maximum velocity so that our player won't endlessly increase their speed.
        }

        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
		{
            TransformComponent transform = GetComponent<TransformComponent>();
            transform.Location = new Vector3(transform.Location.X, transform.Location.Y + 10.0f, transform.Location.Z - 0.7f);


            m_Rb.MaxLinearVelocity = Mathf.Clamp(m_Rb.MaxLinearVelocity + 0.01f, 0.0f, 25.0f); // Slowly increasing the players max speed.

            if (IsGrounded()) // Only get the jump input if the player is currently on the ground.
            {
                GetInput();
               if (m_ShouldJump) // If space has been pressed this frame - jump
                    m_Rb.AddForce(new Vector3(0.0f, 1.0f, -25.0f / 90.0f).Normalize() * JumpForce, ForceMode.Impulse); // Adding a force to the RB.

            }
            else
            {
                m_Rb.AddForce(Vector3.Down * JumpForce / 15.0f, ForceMode.Impulse); // if the player has jumped, add a heavier gravity
            }
            m_ShouldJump = false; // Should jump is only true for one frame because we do not want to apply the force every frame.
        }

        private void GetInput()
        {

            if (Input.IsKeyClicked(KeyBoardKey.Space))
                m_ShouldJump = true;
        }

        bool IsGrounded()
        {
            // Raycast(origin, direction, maxDistance, hit)
            return Physics.Raycast(Location + new Vector3(0.0f, -1.0f, 1.0f) * 0.51f, new Vector3(0.0f, -1.0f, 1.0f), MaxRayLength, out RaycastHit hitInfo);
        }
        private void OnCollisionBegin(Entity other)
        {
            Log.Info($"{Name} Collider with {other.Name}");
        }
    }
}
