
using System;
using Proof;

namespace KickUp
{
	public class BallMovement : Entity
	{

		public Entity Camera;
		[SeperatorAttribute]
		public float MinKickForce = 5f;
		public float MaxKickForce = 25f;
		public float MaxChargeTime = 1.2f;
		[ToolTipAttribute("How long the charge pauses at 0% and 100% before changing direction.")]
		public float ChargeEndHoldTime = 0.2f;

		[SeperatorAttribute]

		[ClampValueAttribute(0f, 1f)]
		[ShowInEditor]
		public float Loft = 0.45f;
		[ToolTipAttribute("Forward force multiplier when Loft is low. Higher value = faster/flatter kick.")]
		public float LowLoftForwardMultiplier = 1.0f;

		[ToolTipAttribute("Forward force multiplier when Loft is high. Lower value = more upward arc and less forward speed.")]
		public float HighLoftForwardMultiplier = 0.45f;

		[ToolTipAttribute("Upward force multiplier when Loft is low. Lower value = flatter kick.")]
		public float LowLoftUpMultiplier = 0.0f;

		[ToolTipAttribute("Upward force multiplier when Loft is high. Higher value = higher lob kick.")]
		public float HighLoftUpMultiplier = 0.9f;

		[SeperatorAttribute]
		public float BrakeStrength = 18f;
		public float MaxBrakeSpeed = 0.2f;
		[ToolTipAttribute("Brake is multiplied by this when airborne. 0.25 = 75% weaker in air.")]
		[ClampValueAttribute(0f, 1f)]
		public float AirBrakeMultiplier = 0.25f;

		[SeperatorAttribute]
		public float MaxSpeed = 35f;

		[SeperatorAttribute]
		public float GroundNormalThreshold = 0.45f;

		[SeperatorAttribute]
		public bool ShowDebug = true;
		public float DebugRayLength = 4f;
		
		RigidBodyComponent m_RigidBodyComponent;

		private bool m_Charging;
		private float m_ChargeTime;
		private float m_ChargeEndHoldTimer;

		private bool m_Braking;

		private bool m_IsGrounded;
		private Vector3 m_GroundNormal = Mathf.Up;
		private float m_SlopeAngle;

		private ChargeDirection m_ChargeDirection = ChargeDirection.Up;

		private enum ChargeDirection
		{
			Up,
			Down
		}

		
		
		// OnCreate is called once when the Entity that this script is attached to
		// is instantiated in the world at runtime
		void OnCreate()
		{
			m_RigidBodyComponent = GetComponent<RigidBodyComponent>();
		}

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
			UpdateCharge();
			DrawKickDebug();
		}

		// OnPhysicsUpdate is called at a fixed timestep for physics-related logic
		void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
		{
			HandleBrake();
			LimitSpeed();
		}

		public void SetLoft(float loft)
		{
			Loft = Mathf.Clamp(loft,0,1);
		}

		public void StartCharge()
		{
			if(m_Charging)
				return;

			m_Charging = true;
			m_ChargeTime = 0f;
			m_ChargeEndHoldTimer = 0f;
			m_ChargeDirection = ChargeDirection.Up;
		}


		public void ReleaseKick()
		{
			if (!m_Charging)
				return;

			m_Charging = false;
			Kick();
		}

		public void SetBrake(bool braking)
		{
			m_Braking = braking;
		}

		public bool IsGrounded()
		{
			return m_IsGrounded;
		}


		public float GetCharge01()
		{
			if (!m_Charging || MaxChargeTime <= 0f)
				return 0f;

			return Mathf.Clamp(m_ChargeTime / MaxChargeTime,0,1);
		}
		 

		private void UpdateCharge()
		{
			if (!m_Charging || MaxChargeTime <= 0f)
				return;

			if (m_ChargeDirection == ChargeDirection.Up)
				m_ChargeTime += World.GetDeltaTime();
			else
				m_ChargeTime -= World.GetDeltaTime();

			m_ChargeTime = Mathf.Clamp(
				m_ChargeTime,
				0f,
				MaxChargeTime
			);

			bool reachedEnd =
				m_ChargeTime >= MaxChargeTime ||
				m_ChargeTime <= 0f;

			if (!reachedEnd)
			{
				m_ChargeEndHoldTimer = 0f;
				return;
			}

			m_ChargeEndHoldTimer += World.GetDeltaTime();

			if (m_ChargeEndHoldTimer < ChargeEndHoldTime)
				return;

			m_ChargeDirection =
				m_ChargeDirection == ChargeDirection.Up
					? ChargeDirection.Down
					: ChargeDirection.Up;

			m_ChargeEndHoldTimer = 0f;
		}

		private void Kick()
		{
			if (Camera == null)
				return;

			float charge01 = Mathf.Clamp(
				m_ChargeTime / MaxChargeTime,0,1
			);

			float kickPower = Mathf.Lerp(
				MinKickForce,
				MaxKickForce,
				charge01
			);

			Vector3 forward = GetSurfaceForward();

			float forwardMultiplier = Mathf.Lerp(
				LowLoftForwardMultiplier,
				HighLoftForwardMultiplier,
				Loft
			);

			float upwardMultiplier = Mathf.Lerp(
				LowLoftUpMultiplier,
				HighLoftUpMultiplier,
				Loft
			);

			Vector3 impulse =
				forward * kickPower * forwardMultiplier +
				m_GroundNormal * kickPower * upwardMultiplier;

			m_RigidBodyComponent.AddForce(
				impulse,
				ForceMode.Impulse
			);
		}


		Vector3 GetCameraRelativeDirection(Vector3 input)
		{
			if (Camera == null)
				return input;

			Vector3 camForward = Camera.Transform.Forward;
			Vector3 camRight = Camera.Transform.Right;

			camForward.y = 0f;
			camRight.y = 0f;

			camForward = camForward.Normalized;
			camRight = camRight.Normalized;

			return camForward * input.z + camRight * input.x;
		}
		private void HandleBrake()
		{
			if (!m_Braking)
				return;

			Vector3 flatVelocity = new Vector3(
				m_RigidBodyComponent.LinearVelocity.x,
				0f,
				m_RigidBodyComponent.LinearVelocity.z
			);

			if (flatVelocity.Magnitude <= MaxBrakeSpeed)
				return;

			float multiplier =
				m_IsGrounded ? 1f : AirBrakeMultiplier;

			Vector3 brakeForce =
				-flatVelocity.Normalized *
				BrakeStrength *
				multiplier;

			m_RigidBodyComponent.AddForce(
				brakeForce,
				ForceMode.Force
			);
		}

		private Vector3 GetSurfaceForward()
		{
			if (Camera == null)
				return Camera.Transform.Forward;

			Vector3 forward = Vector3.ProjectOnPlane(
				Camera.Transform.Forward,
				m_GroundNormal
			);

			if (forward.MagnitudeSqr < 0.001f)
			{
				forward = Vector3.ProjectOnPlane(
					Camera.Transform.Forward,
					m_GroundNormal
				);
			}

			return forward.Normalized;
		}




		private void LimitSpeed()
		{
			Vector3 velocity = m_RigidBodyComponent.LinearVelocity;

			if (velocity.MagnitudeSqr <= MaxSpeed * MaxSpeed)
				return;

			m_RigidBodyComponent.LinearVelocity =
				velocity.Normalized * MaxSpeed;
		}

		private void OnCollisionStay(Collision collision)
		{
			bool foundGround = false;

			for (int i = 0; i < collision.contactCount; i++)
			{
				ContactPoint contact = collision.GetContact(i);

				if (contact.normal.y <= GroundNormalThreshold)
					continue;

				foundGround = true;
				m_GroundNormal = contact.normal;
				m_SlopeAngle = Vector3.Angle(
					m_GroundNormal,
					Vector3.up
				);

				break;
			}

			if (foundGround)
				m_IsGrounded = true;
		}

		private void OnCollisionExit(Collision collision)
		{
			m_IsGrounded = false;
			m_GroundNormal = Vector3.up;
			m_SlopeAngle = 0f;
		}

		public void AddLoft(float input)
		{
			Loft += input;
			Loft = Mathf.Clamp(Loft,0,1);
		}
		private void DrawKickDebug()
		{
			if (!ShowDebug || !m_Charging || Camera == null)
				return;

			Vector3 forward = GetSurfaceForward();

			float forwardMultiplier = Mathf.Lerp(
				LowLoftForwardMultiplier,
				HighLoftForwardMultiplier,
				Loft
			);

			float upwardMultiplier = Mathf.Lerp(
				LowLoftUpMultiplier,
				HighLoftUpMultiplier,
				Loft
			);

			Vector3 kickDirection =
				forward * forwardMultiplier +
				m_GroundNormal * upwardMultiplier;

			if (kickDirection.MagnitudeSqr > 0.001f)
				kickDirection = kickDirection.Normalized;

			DebugRenderer.DrawRay(
				Transform.Location,
				kickDirection *
				DebugRayLength *
				Mathf.Max(GetCharge01(), 0.2f),
				Colors.Yellow
			);
		}
	}
}



/*
 *public class CharacterMovement : Entity
	{
		public float moveForce = 12f;
		public float maxSpeed = 12f;
		public float brakeStrength = 8f;
		public Entity Camera;

		RigidBodyComponent m_RigidBodyComponent;
		
		// OnCreate is called once when the Entity that this script is attached to
		// is instantiated in the world at runtime
		void OnCreate()
		{
			m_RigidBodyComponent = GetComponent<RigidBodyComponent>();
		}

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
		}

		// OnPhysicsUpdate is called at a fixed timestep for physics-related logic
		void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
		{
			float horizontal = Input.GetAxisRaw(InputAxis.Horizontal); // A/D
			float vertical = Input.GetAxisRaw(InputAxis.Vertical);     // W/S

			Vector3 input = new Vector3(horizontal, 0f, vertical).Normalized;

			if (input.MagnitudeSqr > 0.01f)
			{
				Vector3 moveDirection = GetCameraRelativeDirection(input);
				m_RigidBodyComponent.AddForce(moveDirection * moveForce, ForceMode.Force);
			}

			LimitSpeed();

			if (Input.IsKeyPressed(KeyBoardKey.Space))
			{
				Brake();
			}

		}


		Vector3 GetCameraRelativeDirection(Vector3 input)
		{
			if (Camera == null)
				return input;

			Vector3 camForward = Camera.Transform.Forward;
			Vector3 camRight = Camera.Transform.Right;

			camForward.y = 0f;
			camRight.y = 0f;

			camForward = camForward.Normalized;
			camRight = camRight.Normalized;

			return camForward * input.z + camRight * input.x;
		}

		void LimitSpeed()
		{
			Vector3 flatVelocity = new Vector3(m_RigidBodyComponent.LinearVelocity.x, 0f, m_RigidBodyComponent.LinearVelocity.z);

			if (flatVelocity.Magnitude > maxSpeed)
			{
				Vector3 limitedVelocity = flatVelocity.Normalized * maxSpeed;
				m_RigidBodyComponent.LinearVelocity = new Vector3(limitedVelocity.x, m_RigidBodyComponent.LinearVelocity.y, limitedVelocity.z);
			}
		}

		void Brake()
		{
			Vector3 flatVelocity = new Vector3(m_RigidBodyComponent.LinearVelocity.x, 0f, m_RigidBodyComponent.LinearVelocity.z);
			Vector3 brakeForce = -flatVelocity * brakeStrength;

			m_RigidBodyComponent.AddForce(brakeForce, ForceMode.Force);
		}
	}
 */
