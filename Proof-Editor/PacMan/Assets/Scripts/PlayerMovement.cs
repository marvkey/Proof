
using System;
using System.Linq;
using System.Security.Principal;
using System.Threading;
using Proof;
using static PacMan.PlayerMovement;

namespace PacMan
{
	public class PlayerMovement : Entity
	{
		public float MoveSpeed = 5f;
		public float RotationSpeed = 5f;
        float m_TargetYRotation = 0.0f; // target Y Rotation after turn
        bool m_IsRotating = false;
        public float RayDistance =1.0f;  // Distance for the raycast
        public float OffsetRayRadius = 2f;
        private Entity m_HitGridPos;
        RigidBodyComponent m_RigidBody;
        public enum TurnDirection
        {
            Left, 
            Right,
            Back
        }

        
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{
            m_TargetYRotation = Transform.Rotation.y;
            m_RigidBody = GetComponent<RigidBodyComponent>();

        }



        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
		{
            HandleRotation();
            
            foreach (TurnDirection direction in Enum.GetValues(typeof(TurnDirection)))
            {
                Vector4 color = new Vector4(0);
                Vector3 turnDirection = new Vector3(0);
                Vector3 startLocation = Transform.Location;
                switch (direction)
                {
                    case TurnDirection.Left:
                        turnDirection = -Transform.Right;
                        color = Colors.Red;
                        startLocation += -Transform.Right * OffsetRayRadius; // Offset for the corner
                        break;
                    case TurnDirection.Right:
                        turnDirection = Transform.Right;
                        color = Colors.Yellow;
                        startLocation += Transform.Right * OffsetRayRadius; // Offset for the corner
                        break;
                    case TurnDirection.Back:
                        turnDirection = -Transform.Forward;
                        color = Colors.Green;
                        startLocation += -Transform.Forward * OffsetRayRadius; // Offset for the corner
                        break;
                }

                // Raycast in the turn direction
                DebugRenderer.DrawRay(startLocation, turnDirection * RayDistance, color); // Debug ray
            }


           
        }
        void HandleRotation()
        {
            if (!m_IsRotating) return;

            Quaternion targetRotation = new Quaternion (new Vector3(0, Mathf.DegreesToRadians(m_TargetYRotation),0));
            // Smoothly rotate towards the target rotation
            m_RigidBody.Rotation = Quaternion.Lerp(m_RigidBody.Rotation, targetRotation, World.GetDeltaTime() * RotationSpeed);
            // Check if we're close enough using Quaternion.Dot
            float dot = Quaternion.Dot(m_RigidBody.Rotation, targetRotation);
            float angleDifference = Mathf.Acos(dot) * 2; // Angle difference in radians

            if (angleDifference < 0.01) // If the angle is small enough
            {
                m_RigidBody.Rotation = targetRotation; // Snap to target
                m_IsRotating = false; // Allow new input
                Log.Info("Rotation complete!");

            }
           
        }

        public void Move(Vector2 axis)
        {
            Log.Info($"Pos {axis.ToString()}");
            MoveForwardAndBackward(axis.y);
            RotatePlayer(axis.x);
        }
        // x > 0 fowrd x< 0 backwards
        void MoveForwardAndBackward(float x)
        {
            if(x>0 && m_IsRotating == false)
                m_RigidBody.Translate(Transform.Forward * MoveSpeed * x * World.GetDeltaTime());
            else if ( x < 0 && m_IsRotating == false)
            {
                if(CanTurn(TurnDirection.Back))
                {
                    Log.Info("Turn Back!");

                    m_IsRotating = true;
                    m_TargetYRotation += 180;
                }
            }
        }

        // not axis but easier 
        void RotatePlayer(float y)
        {
            if (m_IsRotating) return; // Prevent new rotation commands while already rotating

            if (y > 0 && CanTurn(TurnDirection.Right))
            {
                Vector3 newLocation = new Vector3(m_RigidBody.Location);
                newLocation.z = m_HitGridPos.Location.z;
                newLocation.x = m_HitGridPos.Location.x;
                m_RigidBody.Location = newLocation;

                m_IsRotating = true;
                m_TargetYRotation -= 90;
                Log.Info($"Turn Right! due to {m_HitGridPos.Name} and {m_HitGridPos.Location} ");
            }
            else if (y < 0 && CanTurn(TurnDirection.Left))
            {

                Vector3 newLocation = new Vector3(m_RigidBody.Location);
                newLocation.z = m_HitGridPos.Location.z;
                newLocation.x = m_HitGridPos.Location.x;
                m_RigidBody.Location = newLocation;
                m_IsRotating = true;
                m_TargetYRotation += 90;
                Log.Info($"Turn Left! due to {m_HitGridPos.Name} and {m_HitGridPos.Location} ");

            }
          
        }
     
        bool CanTurn(TurnDirection direction)
        {
            Vector3 turnDirection = new Vector3(0);
            Vector3 startLocation = Transform.Location;

            // Add an offset to start at a corner of the sphere
            switch (direction)
            {
                case TurnDirection.Left:
                    turnDirection = -Transform.Right;
                    startLocation += -Transform.Right * OffsetRayRadius; // Offset for the corner
                    break;
                case TurnDirection.Right:
                    turnDirection = Transform.Right;
                    startLocation += Transform.Right * OffsetRayRadius; // Offset for the corner
                    break;
                case TurnDirection.Back:
                    turnDirection = -Transform.Forward;
                    startLocation += -Transform.Forward * OffsetRayRadius; // Offset for the corner
                    break;
            }
            // Raycast in the turn direction

            // Return true if no wall is detected


            RaycastData data = new RaycastData();
            data.Origin = startLocation;
            data.Direction = turnDirection;
            data.MaxDistance = RayDistance;


            if(Physics.RayCastLayer(data, "GridPos", out RaycastHit hit))
            {
                m_HitGridPos = hit.Entity;

                return true;
            }

            return false;
        }
    }
}
