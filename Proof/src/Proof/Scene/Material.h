#pragma once

#include "Proof/Core/Core.h"
#include "Proof/Math/Math.h"
namespace Proof{
	struct Proof_API Material {
		float Metallness =0.0f;// also shinines
		float Roughness = 0.0f;
		Vector Colour {1,1,1};

		UUID AlbedoTexture;
		UUID NormalTexture;
		UUID MetallicTexture;
		UUID RoughnesTexture;
	};
	enum class CombineMode 
	{
		Average =0,
		Min = 1,
		Mutltiply = 2,
		Max =3
	};
	struct Proof_API PhysicsMaterial {
		float StaticFriction = 0.6f;
		float DynamicFriction = 0.6f;
		float Bounciness = 0.0f;

		CombineMode FrictionCombineMode = CombineMode::Average;
		CombineMode BouncinessCombineMode = CombineMode::Average;
	private:
		void* m_RuntimeBody = nullptr;
		friend class PhysicsEngine;
		friend class PhysicsActor;
	};
}