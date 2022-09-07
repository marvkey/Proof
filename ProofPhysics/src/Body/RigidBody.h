#pragma once
#include "../../../Proof/src/Proof/Math/Vector.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_Vector.hpp>
#include<glm/gtx/Vector_angle.hpp>
namespace ProofPhysicsEngine {
	static inline glm::mat3 _transformInertiaTensorWorld(const glm::quat& q,const glm::mat3& iitBody)
	{
		return iitBody * glm::mat3(q);
	}
	struct RigidBody {
		void Update(float deltaTime) {
			// calculate linear acceleration from force inputs
			m_LastFrameAcceleration = m_Acceleration;
			m_LastFrameVelocity = Velocity;
			m_LastFrameAcceleration += m_ForceAccum * (1 / m_Mass);

			// angular acceleration
			Proof::Vector   angularAcceleration = _transformInertiaTensorWorld(m_Quat, m_InverseInertiaTensor) * m_TorqueAccum; // INVERSE INERTIA TENSOR WORLD SPACE
			
			// Adjust velocities
			// Update linear velocity from both acceleration and impulse
			Velocity += (m_LastFrameAcceleration * deltaTime);
			RotationalVelocity += (angularAcceleration * deltaTime);
			
			// IMPOSE DRAG
			
			//Velocity *= pow(Drag,deltaTime);
			//RotationalVelocity *= pow(AngularDrag,deltaTime );

			// Change  position
			Location += Velocity * deltaTime;
			Rotation += RotationalVelocity * deltaTime;
			
			// add more drag
			m_Quat += glm::quat(glm::vec3{ glm::radians(RotationalVelocity.X), glm::radians(RotationalVelocity.Y), glm::radians(RotationalVelocity.Z) }) * deltaTime;
			
			m_Quat = glm::normalize(m_Quat);

			// FORMULA ACCELRATION( change in veolocity/time)
			m_Acceleration = glm::vec3(m_LastFrameVelocity - Velocity) / deltaTime;
			ClearForceAccum();
		}
		/**
		* Holds the amount of damping applied to angular
		* motion. Damping is required to remove energy added
		* through numerical instability in the integrator.
		*/
		float AngularDrag =0.05;
		float Drag =0;
		bool Gravity = true;
		/**
		 * Holds the linear position of the rigid body in
		 * world space.
		 */
		Proof::Vector   Location{ 0,0,0 };
		/**
		* Holds the angular velocity, or rotation, or the
		* rigid body in world space.
		*/
		Proof::Vector   Rotation{ 0,0,0 };

		

		Proof::VectorTemplate<bool> FreezeLocation{ 0,0,0 };
		Proof::VectorTemplate<bool> FreezeRotation{ 0,0,0 };

		glm::mat3x4 GetTransform() {
			glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3{ glm::radians(Rotation.X), glm::radians(Rotation.Y), glm::radians(Rotation.Z) }));
			return glm::translate(glm::mat4(1.0f), { Location })
				* rotation;
		}

		void AddForce(const Proof::Vector  & force) {
			m_ForceAccum += force;
			m_IsAwake = true;
		}
		//void AddForceAtBodyPoint(const Proof::Vector  & force,const Proof::Vector  & point);
		void SetMass(float value) {
			m_Mass = value;
		}
		void SetMassInfinite(bool infinite) {
			m_MassInfinite = infinite;
		}

		float GetMass() {
			//if (m_MassInfinite)
			//	return 0; // infinite
			return m_Mass;
		}

		bool IsMassInfinite() {
			return m_MassInfinite;
		}
		bool SetAwake(bool awake) {
			m_IsAwake = awake;
		}
		bool GetAwake() {
			return m_IsAwake;
		}
		void AddTorque(const Proof::Vector  & torque) {
			m_TorqueAccum += torque;
			m_IsAwake = true;
		}
		/**
		 * Some bodies may never be allowed to fall asleep.
		 * User controlled bodies, for example, should be
		 * always awake.
		 */
		bool CanSleep;
		glm::quat GetRotationQuat() {
			return glm::quat(glm::vec3{ glm::radians(Rotation.X), glm::radians(Rotation.Y), glm::radians(Rotation.Z) });
		}
	private:
		glm::quat m_Quat = glm::quat(glm::vec3{ glm::radians(Rotation.X), glm::radians(Rotation.Y), glm::radians(Rotation.Z) });
		Proof::Vector   m_LastFrameAcceleration;
		void SetInertiaTensor(const glm::mat3& inertiaTensor) {
			m_InverseInertiaTensor = glm::inverse(inertiaTensor);
		}
		
		void ClearForceAccum() {
			m_ForceAccum = Proof::Vector  { 0,0,0 };
			m_TorqueAccum = Proof::Vector  { 0,0,0 };
		}
		float GetInverseMass() {
			return 1 / m_Mass;
		}
		Proof::Vector   m_ForceAccum;
		Proof::Vector   m_Acceleration;
		Proof::Vector   m_TorqueAccum;
		float m_Mass = 1;
		bool m_MassInfinite =false;

		/**
		 * Holds the linear velocity of the rigid body in
		 * world space.
		 */
		Proof::Vector   Velocity{ 0,0,0 };
		Proof::Vector   RotationalVelocity{ 0,0,0 };
		Proof::Vector   m_LastFrameVelocity;
		/**
		* Holds the inverse of the body’s inertia tensor. The inertia
		* tensor provided must not be degenerate (that would mean
		* the body had zero inertia for spinning along one axis).
		* As long as the tensor is finite, it will be invertible.
		* The inverse tensor is used for similar reasons as those
		* for the use of inverse mass.
		*
		* The inertia tensor, unlike the other variables that define
		* a rigid body, is given in body space.
		*
		* @see inverseMass
		*/

		glm::mat3 m_InverseInertiaTensor;

		bool m_IsAwake = true;
	};
}