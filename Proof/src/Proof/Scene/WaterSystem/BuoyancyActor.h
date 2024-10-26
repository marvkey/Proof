#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/UUID.h"
#include "Proof/Scene/Entity.h"

namespace Proof
{
#if 0
	struct Floater
	{
		float SubmersionDepth = 1.0f;
		float Drag = 0.99f;
		float AngularDrag = 0.5f;
		float BuoyancyStrength = 3.0f;
	};
	struct BuoyancyFloater
	{
		UUID EntityUUID;
		Floater Floater;
	};

	
	
	class BuoyancyActor : RefCounted
	{
	public:
		BuoyancyActor();
		std::vector<BuoyancyFloater> Floaters;
		 
		std::vector<BuoyancyFloater> GetActiveFloaters();

		void OnUpdate(float deltaTime);

		void SetEntity(Entity entity);

	};
#endif
}