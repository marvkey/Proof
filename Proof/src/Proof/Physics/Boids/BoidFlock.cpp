#include "Proofprch.h"
#include "BoidFlock.h"
#include "Proof/Physics/PhysicsActor.h"
#include "Proof/Math/Random.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/World.h"
#include "Proof/Physics/PhysicsWorld.h"
#include "Proof/Scene/SceneUtils.h"

namespace Proof
{

	void BoidFlock::OnUpdate(float deltaTime)
	{
		PF_PROFILE_FUNC();

	}

	void BoidFlock::AddBoidEntity(UUID entityID)
	{
			m_Boids[entityID] = Count<Boid>::Create(this,entityID);
	}

	void BoidFlock::RemoveBoidEntity(UUID entityID)
	{
		if(m_Boids.find(entityID) != m_Boids.end())
			m_Boids.erase(entityID);
	}

	TransformComponent GetWorldTransformComponent(Entity entity)
	{
		return entity.GetCurrentWorld()->GetWorldSpaceTransformComponent(entity);
	}

	Boid::Boid(Count<BoidFlock> flock, UUID entityID)
		: m_Flock(flock.Get()), m_ID(entityID)
	{
	
		auto settings = flock->GetSettings();
		float startSpeed = (settings.MinSpeed + settings.MaxSpeed) / 2.0f;
		m_Velocity = GetEntity().GetTransformComponent().GetFowardVector() * startSpeed;
		m_Forward  = GetEntity().GetTransformComponent().GetFowardVector();
	}
	inline float UniqueIDHash(uint64_t id)
	{
		id ^= (id >> 33);
		id *= 0xff51afd7ed558ccdULL;
		id ^= (id >> 33);
		id *= 0xc4ceb9fe1a85ec53ULL;
		id ^= (id >> 33);
		return (id & 0xFFFFFF) / float(0xFFFFFF);
	}
	void Boid::OnUpdate(float deltaTime)
	{
		PF_PROFILE_FUNC();

		m_Acceleration = glm::vec3(0.0f);

		auto flock = m_Flock.Lock();
		Entity target = GetPhysicsWorld()->GetWorld()->TryGetEntityWithUUID(flock->GetSettings().TargetEntity);
		m_Acceleration += 0.2f * Random::InsideUnitSphere();

		if (target.IsValid())
		{
			if (flock->GetSettings().Orbit)
			{
#if 0 // this spwns a full obrit aroudnthe target right now oly want to spawn orbit on the top half 
				auto playerPos = target.GetTransformComponent().Location;
    
				const float orbitRadius = flock->GetSettings().OrbitRadius;
				const float wander = flock->GetSettings().OrbitWandering;

				// Unique per-boid base angle
				float seed = UniqueIDHash(m_ID);

				// Create random spherical coordinates (stable per boid)
				float theta = seed * glm::two_pi<float>();           // horizontal angle [0, 2π)
				float phi   = glm::mix(0.3f, 2.8f, seed);            // vertical spread [~17°, ~160°]

				// Make the swarm rotate over time
				float rotationSpeed = 0.3f;
				float t = FrameTime::GetTime() * rotationSpeed;

				// Add gentle time rotation to keep motion alive
				theta += t * 0.5f;
				phi   += glm::sin(t + seed * 6.2831f) * 0.1f; // subtle vertical breathing

				// Convert spherical to Cartesian (relative to player)
				glm::vec3 offset = glm::vec3(
					orbitRadius * glm::sin(phi) * glm::cos(theta),
					orbitRadius * glm::cos(phi),
					orbitRadius * glm::sin(phi) * glm::sin(theta)
				);

				// Add optional wandering (small random offset)
				offset += Random::InsideSphere(wander);

				// Final target position around player
				glm::vec3 orbitPos = playerPos + offset;

				// Steer toward orbit point
				glm::vec3 offsetToTarget = orbitPos - GetEntity().GetTransformComponent().Location;
				m_Acceleration = SteerTowards(offsetToTarget) * flock->GetSettings().TargetWeight;
#endif

				auto playerPos = target.GetTransformComponent().Location;

				const float orbitRadius = flock->GetSettings().OrbitRadius;
				const float wander = flock->GetSettings().OrbitWandering;

				// Unique per-boid seed
				float seed = UniqueIDHash(m_ID);

				// --- Angle limits ---
				const float minHorizontalAngle = flock->GetSettings().OrbitMinAngleRadians;
				const float maxHorizontalAngle = flock->GetSettings().OrbitMaxAngleRadians;
				const float minVerticalAngle = flock->GetSettings().OrbitMinVerticalAngleRadians;
				const float maxVerticalAngle = flock->GetSettings().OrbitMaxVerticalAngleRadians;

				// Random initial position within the spherical segment
				float theta = glm::mix(minHorizontalAngle, maxHorizontalAngle, UniqueIDHash(m_ID + 123));
				float phi = glm::mix(minVerticalAngle, maxVerticalAngle, UniqueIDHash(m_ID + 456));

				// --- Smooth rotation over time ---
				float rotationSpeed = 0.1f;
				float t = FrameTime::GetTime() * rotationSpeed;

				// Animate horizontal rotation and subtle vertical variation
				theta += t * flock->GetSettings().OrbitHorizontalSpeed;
				phi += glm::sin(t * flock->GetSettings().OrbitVerticalSpeed + seed * glm::two_pi<float>()) 
					   * flock->GetSettings().OrbitVerticalBobbing;

				// Keep angles within bounds
				theta = fmod(theta, glm::two_pi<float>());
				if (theta < minHorizontalAngle) theta += (maxHorizontalAngle - minHorizontalAngle);
				if (theta > maxHorizontalAngle) theta -= (maxHorizontalAngle - minHorizontalAngle);

				phi = glm::clamp(phi, minVerticalAngle, maxVerticalAngle);

				// --- Convert spherical → Cartesian ---
				glm::vec3 offset = glm::vec3(
					orbitRadius * glm::sin(phi) * glm::cos(theta),
					orbitRadius * glm::cos(phi),
					orbitRadius * glm::sin(phi) * glm::sin(theta)
				);

				// --- Add gentle random wandering ---
				offset += Random::InsideSphere(wander);

				// --- Final orbit position ---
				glm::vec3 orbitPos = playerPos + offset;

				// --- Steer toward orbit point ---
				glm::vec3 offsetToTarget = orbitPos - GetEntity().GetTransformComponent().Location;
				m_Acceleration = SteerTowards(offsetToTarget) * flock->GetSettings().TargetWeight;
				
			}
			else
			{
				auto offsetToTarget = target.GetTransformComponent().Location -  GetEntity().GetTransformComponent().Location;

				m_Acceleration = SteerTowards(offsetToTarget) * flock->GetSettings().TargetWeight;
			}
		}

		
		if(m_NumPerceivedFlockmates != 0)
		{
			m_CentreOfFlockmates /= m_NumPerceivedFlockmates;

			glm::vec3 offsetToFlockmatesCentre = (m_CentreOfFlockmates - GetEntity().GetTransformComponent().Location);

			auto alignmentForce = SteerTowards(m_AvgFlockHeading) * flock->GetSettings().AlignWeight;
			auto cohesionForce = SteerTowards(offsetToFlockmatesCentre) * flock->GetSettings().CohesionWeight;
			auto seperationForce = SteerTowards(m_AvgAvoidanceHeading) * flock->GetSettings().SeperateWeight;

			m_Acceleration += alignmentForce;
			m_Acceleration += cohesionForce;
			m_Acceleration += seperationForce;
		}
		
		if (IsHeadingForCollision())
		{
			glm::vec3 collisionAvoidDir = ObstacleRays();
			glm::vec3 collisionAvoidForce = SteerTowards(collisionAvoidDir) * flock->GetSettings().AvoidCollisionWeight;
			m_Acceleration += collisionAvoidForce;
		}
		// Integrate acceleration
		if (!glm::any(glm::isnan(m_Acceleration)))
			m_Velocity += m_Acceleration * deltaTime;

		float speed = glm::length(m_Velocity);
		if (speed < 1e-6f || glm::any(glm::isnan(m_Velocity)))
		{
			speed = flock->GetSettings().MinSpeed;
			m_Velocity = speed * m_Forward;
		}
		
		glm::vec3 dir = m_Velocity / speed;

		speed = glm::clamp(speed, flock->GetSettings().MinSpeed, flock->GetSettings().MaxSpeed);
		m_Velocity = dir *speed;

		GetEntity().GetTransformComponent().Location += m_Velocity * deltaTime;


		BoidFacingMode faceMode = flock->GetSettings().FacingMode;

		if (!target.IsValid() && ( flock->GetSettings().FacingMode == BoidFacingMode::AlwaysTarget || flock->GetSettings().FacingMode == BoidFacingMode::BlendMovementTarget))
		{
			PF_EC_WARN("{} boid Facing mode requires a valid target", GetEntity().GetName());
			faceMode = BoidFacingMode::None;
		}
		
		switch (faceMode)
		{
			case BoidFacingMode::None:
			break;
			
			case BoidFacingMode::MovementDirection:
				{
					glm::quat orientation  = glm::quatLookAt(dir, Math::GetUpVector());
					GetEntity().GetTransformComponent().SetRotation(orientation);
					break;
				}

			case BoidFacingMode::AlwaysTarget:
				{
					glm::vec3 targetDir = glm::normalize(target.GetTransformComponent().Location - GetEntity().GetTransformComponent().Location);
					glm::quat orientation  = glm::quatLookAt(targetDir, Math::GetUpVector());
					GetEntity().GetTransformComponent().SetRotation(orientation);
					break;
				}

			case BoidFacingMode::BlendMovementTarget:
				{
					glm::vec3 targetDir = glm::normalize(target.GetTransformComponent().Location - GetEntity().GetTransformComponent().Location);
					glm::vec3 desiredLookDir = glm::mix(dir, targetDir, 0.7f);
					glm::quat orientation = glm::quatLookAt(desiredLookDir, Math::GetUpVector());
					GetEntity().GetTransformComponent().SetRotation(orientation);
					m_Forward = desiredLookDir;
					break;
				}

		}

		m_Forward = dir;
		
		
#if 0
		// If velocity magnitude is invalid or zero, reset to safe forward direction
		if (speed < 1e-6f || glm::any(glm::isnan(m_Velocity)))
		{
			glm::vec3 accelDir;
			if (glm::length2(acceleration) > 1e-6f)
			{
				accelDir = glm::normalize(acceleration);
			}
			else if (target.IsValid())
			{
				accelDir = glm::normalize(target.GetTransformComponent().Location -  m_PhysicsActor->GetLocation());
				
			}
			else
			{
				accelDir = m_Forward;
			}

			// Use acceleration direction if available, otherwise forward
			m_Velocity = accelDir * flock->GetSettings().MinSpeed;
			speed = glm::length(m_Velocity);
		}

		// Normalize safely
		glm::vec3 dir =  glm::normalize(m_Velocity);

		// Clamp to min/max speed range
		speed = glm::clamp(speed, flock->GetSettings().MinSpeed, flock->GetSettings().MaxSpeed);
		m_Velocity = dir * speed;

		// --- Apply movement & rotation ---
		m_PhysicsActor->Translate(m_Velocity * deltaTime);
		m_Forward = dir;

		glm::vec3 up = Math::GetUpVector();
		m_PhysicsActor->SetRotation(glm::quatLookAt(dir, up));
#endif
	}


	glm::vec3 NormalizeSafe(glm::vec3 const &v)
	{

		float length = glm::length(v);
		return length > 0.0f ? glm::vec3(v / length) : glm::vec3(v.x,v.y,v.z);
	}
	inline glm::vec3 ClampMagnitude(const glm::vec3& vector, float maxLength)
	{
		float len = glm::length(vector);
		
		if (len <= maxLength)
			return vector;
		return NormalizeSafe(vector) * maxLength;
	}

	glm::vec3 Boid::SteerTowards(const glm::vec3& vector)
	{
		PF_PROFILE_FUNC();

		glm::vec3 v = NormalizeSafe(vector) * m_Flock.Lock()->GetSettings().MaxSpeed - m_Velocity;
		return ClampMagnitude(v, m_Flock.Lock()->GetSettings().MaxSteerForce);
	}

	bool Boid::IsHeadingForCollision()
	{
		PF_PROFILE_FUNC();

		RaycastHit hit;


		if (GetPhysicsWorld()->SphereCast(GetEntity().GetTransformComponent().Location,
			m_Forward, m_Flock.Lock()->GetSettings().BoundsRadius,
			m_Flock.Lock()->GetSettings().CollisionAvoidDst,
			&hit))
		{
			return true;
		}

		return false;
	}

	struct BoidHelper
	{
		static inline const int NUM_VIEW_DIRECTIONS = 300;
		static inline glm::vec3 ViewDirections[NUM_VIEW_DIRECTIONS];
		static void Initilize()
		{
			if(m_Initialized)
				return;
			float goldenRatio = (1 + glm::sqrt(5.0f)) / 2;
			float angleIncrement = glm::pi<float>() * 2 * goldenRatio;

			for (int i = 0; i < NUM_VIEW_DIRECTIONS; i++) {
				float t = (float)i / NUM_VIEW_DIRECTIONS;
				float inclination = glm::acos(1 - 2 * t);
				float azimuth = angleIncrement * i;

				float x = glm::sin(inclination) * glm::cos(azimuth);
				float y = glm::sin(inclination) * glm::sin(azimuth);
				float z = glm::cos (inclination);
				ViewDirections[i] = glm::vec3(x, y, z);
			}

			m_Initialized = true;
		}

	private:
		
		static inline bool m_Initialized = false;
	};

	glm::vec3 Boid::ObstacleRays()
	{
		PF_PROFILE_FUNC();

		BoidHelper::Initilize();
		for(int i = 0; i < BoidHelper::NUM_VIEW_DIRECTIONS; i++)
		{
			glm::vec3 dir = GetEntity().GetTransformComponent().GetRotation() * BoidHelper::ViewDirections[i];
			RaycastHit hit;
			if (!GetPhysicsWorld()->SphereCast(GetEntity().GetTransformComponent().Location,
				dir, m_Flock.Lock()->GetSettings().BoundsRadius,
				m_Flock.Lock()->GetSettings().CollisionAvoidDst,
				&hit))
			{
				return dir;
			}
		}

		return m_Forward;
	}

	class Entity Boid::GetEntity()
	{
		return m_Flock.Lock()->GetWorld()->GetEntity(m_ID);
	}

	Count<class PhysicsWorld> Boid::GetPhysicsWorld()
	{
		return GetEntity().GetCurrentWorld()->GetPhysicsWorld();
	}
}
