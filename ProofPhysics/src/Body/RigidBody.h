#pragma once
#include "../../../Proof/src/Proof/Resources/Math/Vector.h"
namespace ProofPhysicsEngine {
	struct RigidBody {
		uint64_t Mass = 1;
		bool Gravity = true;

		Proof::Vector<bool> FreezeLocation{ 0,0,0 };
		Proof::Vector<bool> FreezeRotation{ 0,0,0 };
	};
}