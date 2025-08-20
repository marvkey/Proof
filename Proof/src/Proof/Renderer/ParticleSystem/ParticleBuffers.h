#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Math/Math.h"
#include <glm/glm.hpp>

namespace Proof
{
	struct alignas(16) SBParticleInitalState
	{
		float Duration;
		int bLooping;
		float StartLifetime;
		float StartSpeed;

		glm::vec3 StartSize = glm::vec3(1.0f);
		float GravityModifier =0.0f;

		glm::vec4 StartColor = glm::vec4(1.0f);

		glm::vec3 EmitterPosition = glm::vec3(0.0f);
		float FadeOutSpeed = 0.5; // particle duration over how fast remaingin particles fade out

		glm::vec3 EmitterPrevPosition = glm::vec3(0.0f);
	};

	// ----------DONT USE ALGIN AS FOR THIS ONES AS THEY GET PACKED LATER INOT ONE BIG STRUCT THEY EACH MUST COVER 1 FULL PACK (SO 4 BYTES OFFSET //
	struct UBParticleEmission
	{
		float ParticlesPerSecond = 10; // particle emit per second
		float ParticlesPerDistance = 0.0f; // particle emit per distance
		int padding0;
		int padding1;
	};

	enum class ParticleEmitterShape
	{
		Sphere = 0
	};

	struct UBParticleShape
	{
		int Shape = 0; //ParticleEmitterShape
		float RandomizeDirection = 0.0f; // min 0, max 1
		float SpherizeDirection = 0.0f; // min 0, max 1
		float RandomizePosition = 0.0f;

		int bEnabled = false;
		int padding0;
		int padding1;
		int padding2;

		// per shape
		float SphereRadius = 1.0f;
		int padding3;
		int padding4;
		int padding5;
	};

	struct UBParticleVelocityOverLifeTime
	{
		glm::vec3 Linear = { 1,1,1 };
		float SpeedModifier;

		glm::vec3 Orbital = glm::vec3(0.0f);
		float Radial;

		glm::vec3 Offset = glm::vec3(0.0f);
		int bEnabled = false;
	};

	struct UBColorOverLifeTime
	{
		int bEnabled = false;
		int padding0;
		int padding1;
		int padding2;

		glm::vec4 FinalColor = glm::vec4(1.0f);
	};
	// ----------DONT USE ALGIN AS FOR THIS ONES AS THEY GET PACKED LATER INOT ONE BIG STRUCT //
	
	struct alignas(16) SBParticleEmitterSettings
	{
		UBParticleEmission Emission;
		UBParticleShape Shape;
		UBParticleVelocityOverLifeTime VelocityOverLifeTime;
		UBColorOverLifeTime ColorOverLifeTime;
	};

	struct alignas(16) SBParticleTrackableData
	{
		float TimeElapsed = 0;
		int ActiveParticles = 0;
		int DeadParticles = 0;
		int MaxParticles = 0; // not edited in teh shader
	};
}