 using System;
using System.Runtime.InteropServices;
using Proof;

namespace NullState
{
    //https://www.youtube.com/watch?v=f473C43s8nE&t=295s

    public class CharacterMovement : Entity
    {

        public float WalkingSpeed = 10;
        public float RunningSpeed = 20;

        public float MoveSpeed
        {
            get { return IsRunning ? RunningSpeed : WalkingSpeed; }
        }
        public float RotationSpeed = 10;
        public float JumpForce = 12;
        public float JumpCoolDown = 0.5f;
        public float AirMultiplier = 0.4f;


        bool m_ReadyToJump = true;
        public Entity Camera;
        public bool IsRunning = false;

        Vector2 m_MoveRate = Vector2.Zero;
        Vector2 m_RotateRate = Vector2.Zero;
        Vector2 m_RotationAccumulator;

        public float PlayerHeight;
        public float GroundedDrag = 0.5f;

        public float MaxSlopeAngle = 40.0f;

        private Vector3 m_MoveDir;
        public enum MovementState
        {
            Walking,
            Sprinting,
            Air
        }

        public MovementState State = MovementState.Walking;
        RigidBodyComponent m_RigidBody;

        Vector3 SlopeHitNormal;
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
        {
            m_RigidBody = GetComponent<RigidBodyComponent>();
        }

        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
        {
            StateHandler();
            UpdateRotate(m_RotateRate);
            SpeedControl();
        }

        // OnPhysicsUpdate is called at a fixed timestep for physics-related logic
        void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
        {
            if (m_RigidBody == null)
                return;
            MoveCharacter();
            ApplyPlayerDrag(fixedPhysicsDeltaTime);
        }

       
        public void Move(Vector2 axis)
        {
            m_MoveRate += axis;
        }

        public void Rotate(Vector2 axis)
        {
            m_RotateRate += axis;
        }

        public void Jump()
        {
            if (IsGrounded() == false || m_ReadyToJump == false)
                return;


            Vector3 v = m_RigidBody.Velocity;

            m_RigidBody.Velocity = new Vector3(v.x, 0f, v.z); // to make sure we always jump same height
            m_RigidBody.AddForce(Transform.Up * JumpForce, ForceMode.Impulse);

            Log.Trace("Jumped");

            m_ReadyToJump = false;
            Invoke(ResetJump, JumpCoolDown);

        }
        
        void ResetJump()
        {
            m_ReadyToJump = true;
        }
        public bool IsGrounded()
        {

            RaycastData data = new RaycastData();
            data.Origin = Transform.Location;
            data.Direction = Mathf.Down;
            data.MaxDistance = PlayerHeight * 0.5f + 0.2f;

            data.ExcludedEntities = new ulong[1];
            data.ExcludedEntities[0] = ID;

            if (Physics.RayCast(data, out RaycastHit hit))
            {
                return true;
            }

            return false;
        }

        void UpdateRotate(Vector2 rotate)
        {
            if (rotate == Vector2.Zero)
                return;

            float x = m_RotateRate.x * RotationSpeed * World.GetDeltaTime();
            float y = m_RotateRate.y * RotationSpeed * World.GetDeltaTime();

            m_RotationAccumulator.x -= y;
            m_RotationAccumulator.y += x;

            m_RotationAccumulator.x = Mathf.Clamp(m_RotationAccumulator.x, -90, 90);

            //Camera.Transform.RotationQuat = Quaternion.EularToQuat(new Vector3(Mathf.DegreesToRadians(m_RotationAccumulator.x), 0, 0)); // only works when camera is a child of actor
            // works when not usign camera s a child
            Camera.Transform.RotationQuat = Quaternion.EularToQuat(new Vector3(Mathf.DegreesToRadians(m_RotationAccumulator.x), -Mathf.DegreesToRadians(m_RotationAccumulator.y), 0));

            m_RigidBody.Rotation = Quaternion.EularToQuat(new Vector3(0, -Mathf.DegreesToRadians(m_RotationAccumulator.y), 0));

            m_RotateRate = Vector2.Zero;
        }

        void MoveCharacter()
        {
            if (m_MoveRate == Vector2.Zero)
                return;
            // Combine into one direction vector
            m_MoveDir = (Transform.Forward * m_MoveRate.y) + (Transform.Right * m_MoveRate.x);

            if(OnSlope())
            {
                m_RigidBody.AddForce(GetSlopeMoveDirection() * MoveSpeed * 20f, ForceMode.Force);

                if (m_RigidBody.Velocity.y > 0)
                    m_RigidBody.AddForce(Mathf.Down * 80f, ForceMode.Force);
            }

            if (IsGrounded() == false)
                m_RigidBody.AddForce(m_MoveDir.Normalized * (IsRunning ? RunningSpeed : WalkingSpeed) * 10f * AirMultiplier);
            else
                m_RigidBody.AddForce(m_MoveDir.Normalized * (IsRunning ? RunningSpeed : WalkingSpeed) * 10f);

            m_MoveRate = Vector2.Zero;

            // todo do we want players to slide donw from slope
           /// m_RigidBody.Gravity = !OnSlope();
        }

        void ApplyPlayerDrag(float dt)
        {
            if (IsGrounded() == false)
            {
                m_RigidBody.LinearDrag = 0;

                return;
            }

            m_RigidBody.LinearDrag = GroundedDrag;
        }

        void StateHandler()
        {
            if(IsGrounded() == false)
            {
                State = MovementState.Air;
                return;
            }
            if (IsRunning)
            {
                State = MovementState.Sprinting;
            }
            else
            {
                State = MovementState.Walking;
            }
        }
        void SpeedControl()
        {

            Vector3 flatVel = new Vector3(m_RigidBody.Velocity.x, 0, m_RigidBody.Velocity.z);
            float maxSpeed = MoveSpeed;

            // when walking cant go over speed 
            if (IsRunning == false)
            {
                if (flatVel.Magnitude > maxSpeed)
                {
                    Vector3 limitedVel = flatVel.Normalized * maxSpeed;
                    m_RigidBody.Velocity = new Vector3(limitedVel.x, m_RigidBody.Velocity.y, limitedVel.z);
                }
                return;
            }

            float speed = flatVel.Magnitude;

            // if runnign we are creatingn a fall off you can go over mnax speed but its harder to gain mommentum
            if (speed > maxSpeed)
            {
                // exponent controls curve steepness (2 = quadratic)
                float falloffPower = 2.0f;

                // scale velocity down slightly instead of snapping
                float factor = 1.0f / (1.0f + glm.Pow((speed - maxSpeed) / maxSpeed, falloffPower));

                Vector3 adjustedVel = flatVel * factor;

                m_RigidBody.Velocity = new Vector3(adjustedVel.x, m_RigidBody.Velocity.y, adjustedVel.z);
            }
        }
         
        bool OnSlope()
        {
            RaycastData data = new RaycastData();
            data.Origin = Transform.Location;
            data.Direction = Mathf.Down; 
            data.MaxDistance = PlayerHeight * 0.5f + 0.3f;

            data.ExcludedEntities = new ulong[1];
            data.ExcludedEntities[0] = ID;

            if (Physics.RayCast(data, out RaycastHit hit))
            {
                float angle = Vector3.AngleDegrees(Mathf.Up,hit.Normal);
                 
                SlopeHitNormal = hit.Normal;
                return angle < MaxSlopeAngle && angle != 0; 
            }

            SlopeHitNormal = Vector3.Zero;
            return false;
        }

        Vector3 GetSlopeMoveDirection()
        {
            return Vector3.ProjectOnPlane(m_MoveDir, SlopeHitNormal).Normalized;
        }
    }
}
