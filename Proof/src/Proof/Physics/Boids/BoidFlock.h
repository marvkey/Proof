#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include "../PhysicsLayerManager.h"
#include<glm/gtc/type_ptr.hpp>

namespace Proof
{
	// used in c# so do not change without thinking

	enum class BoidFacingMode 
	{
		None = 0,               // Don't set rotation at all
		MovementDirection,  // Face where they're moving (default)
		AlwaysTarget,       // Always face the target
		BlendMovementTarget  // Blend between movement and target, ( has to have valid target)
	};
	struct BoidFlockSettings
	{
	    // --- Core Movement ---
	    float MinSpeed = 2.0f;            // Prevents stalling
	    float MaxSpeed = 5.0f;            // Max chase/orbit speed
	    float MaxSteerForce = 3.0f;       // How sharp turns can be
		BoidFacingMode FacingMode = BoidFacingMode::MovementDirection; // if you want the boid system to set wher each boid faces

	    // --- Flocking Behavior ---
	    float PerceptionRadius = 2.5f;    // Sensing range for nearby boids
	    float AvoidanceRadius = 1.0f;     // Minimum distance between boids

	    float AlignWeight = 1.0f;         // Strength of alignment (same direction)
	    float CohesionWeight = 1.0f;      // Pull toward flock center
	    float SeperateWeight = 1.0f;      // Push away from close neighbors

	    // --- Target / Following ---
	    float TargetWeight = 1.0f;        // Pull toward target
	    UUID TargetEntity = UUID(0);      // The entity being followed/orbited

	    // --- Collision Avoidance ---
	    PhysicsLayerID LayerID = 0;
	    float BoundsRadius = 0.27f;
	    float AvoidCollisionWeight = 10.0f;
	    float CollisionAvoidDst = 5.0f;

	    // --- Orbiting Behavior ---
	    bool Orbit = false;               // Enable orbiting mode
	    float OrbitRadius = 8.0f;         // Distance to maintain around target
	    float OrbitWandering = 0.5f;      // How much randomness/noise in orbit path
	    
	    // Horizontal angle range (around Y axis)
	    float OrbitMinAngleRadians = 0.0f;
	    float OrbitMaxAngleRadians = glm::two_pi<float>(); // Full circle
	    
	    // Vertical angle range (from Y axis)
	    float OrbitMinVerticalAngleRadians = 0.0f;         // Top of sphere
	    float OrbitMaxVerticalAngleRadians = glm::pi<float>(); // Bottom of sphere

		float OrbitHorizontalSpeed = 0.5f;    // How fast boids orbit horizontally
		float OrbitVerticalSpeed = 0.3f;      // How fast boids bob vertically  
		float OrbitVerticalBobbing = 0.05f;    // How much vertical movement

	    // Helper methods to calculate sphere coverage
	    float GetHorizontalCoverage() const {
	        return (OrbitMaxAngleRadians - OrbitMinAngleRadians) / glm::two_pi<float>();
	    }
	    
	    float GetVerticalCoverage() const {
	        return (OrbitMaxVerticalAngleRadians - OrbitMinVerticalAngleRadians) / glm::pi<float>();
	    }
	    
	    float GetTotalSphereCoverage() const {
	        return GetHorizontalCoverage() * GetVerticalCoverage();
	    }
		/*
		*OrbitMinAngleRadians = 0.0f;
		OrbitMaxAngleRadians = glm::two_pi<float>();
		OrbitMinVerticalAngleRadians = 0.0f;
		OrbitMaxVerticalAngleRadians = glm::pi<float>();
		// Coverage: 100%
		
		Hemisphere (top half):
		cpp
		OrbitMinAngleRadians = 0.0f;
		OrbitMaxAngleRadians = glm::two_pi<float>();
		OrbitMinVerticalAngleRadians = 0.0f;
		OrbitMaxVerticalAngleRadians = glm::half_pi<float>();
		// Coverage: 50%
		Quarter sphere (front-top):

		cpp
		OrbitMinAngleRadians = -glm::quarter_pi<float>();
		OrbitMaxAngleRadians = glm::quarter_pi<float>();
		OrbitMinVerticalAngleRadians = 0.0f;
		OrbitMaxVerticalAngleRadians = glm::half_pi<float>();
		// Coverage: 25%
		*/
	};


	class BoidFlock : public RefCounted
	{
    public:
		BoidFlock()
		{
			
		}

		BoidFlock(Count<BoidFlock> other)
		{
			m_Settings = other->m_Settings;
		}
		void OnUpdate(float deltaTime);
        BoidFlockSettings GetSettings() const { return m_Settings; }
        BoidFlockSettings& GetSettingsRef() { return m_Settings; }
		void SetWorld(Count<class World> world) { m_World = world; }
		Count<class World> GetWorld(){return m_World;}

        void AddBoidEntity(UUID entityID);
        void RemoveBoidEntity(UUID entityID);
    private:
		Count<class World> m_World;
		BoidFlockSettings m_Settings;
		std::unordered_map<UUID, Count<class Boid>> m_Boids;


        friend class BoidManager;
	};

    class Boid : public RefCounted
    {

    public:
        Boid(Count<BoidFlock> flock, UUID ID);
        void OnUpdate(float deltaTime); 
    private:
		glm::vec3 SteerTowards(const glm::vec3& vector);
		bool IsHeadingForCollision();

        glm::vec3 ObstacleRays();

    	class Entity GetEntity();

    	Count<class PhysicsWorld> GetPhysicsWorld();
    private:
    	glm::vec3 m_Velocity = glm::vec3(0.0f);
    	glm::vec3 m_Acceleration = glm::vec3(0.0f);

        glm::vec3 m_AvgFlockHeading = glm::vec3(0.0f);
		glm::vec3 m_AvgAvoidanceHeading = glm::vec3(0.0f);
		glm::vec3 m_CentreOfFlockmates = glm::vec3(0.0f);
        glm::vec3 m_Forward = glm::vec3(0.0f);
		uint32_t m_NumPerceivedFlockmates = 0;
		WeakCount<BoidFlock> m_Flock;
    	UUID m_ID;
        friend class BoidManager;
    };

}