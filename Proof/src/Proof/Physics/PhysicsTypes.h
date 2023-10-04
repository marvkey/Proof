#pragma once
#include <glm/glm.hpp>
namespace Proof
{
	enum class BroadphaseType
	{
		SweepAndPrune,
		MultiBoxPrune,
		AutomaticBoxPrune
	};

	enum class FrictionType
	{
		Patch,
		OneDirectional,
		TwoDirectional
	};
	enum class CollisionDetectionType : uint32_t
	{
		Discrete,
		Continuous,
		ContinuousSpeculative
	};
	struct PhysicsSettings
	{
		glm::vec3 Gravity = { 0.0f, -9.81f, 0.0f };
		float BounceThresholdVelocity = 2; // minimum 0, maxim is max float
		//float SleepThreshold = 0.005;

		uint32_t SolverIterations = 8;
		uint32_t SolverVelocityIterations = 2;

		BroadphaseType BroadPhaseType = BroadphaseType::SweepAndPrune;
		FrictionType FrictionModel = FrictionType::Patch;
	};
}