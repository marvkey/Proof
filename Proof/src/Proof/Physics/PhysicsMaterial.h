#pragma once
#include "Proof/Asset/Asset.h"

namespace Proof
{
	enum class CombineMode
	{
		Average = 0,
		Min = 1,
		Mutltiply = 2,
		Max = 3
	};
	struct PhysicsMaterial : public Asset
	{
		PhysicsMaterial(float staticFriction = 0.6f, float dynamicFriction = 0.6f, float bounciness = 0.0f);
		float GetStaticFriction();
		float GetDynamicFriction();
		float GetBounciness();

		void SetStaticFriction(float friciono);
		void SetDynamicFriction(float friction);
		void SetBounciness(float bounciness);

		CombineMode GetFrictionCombineMode();
		CombineMode GetBouncinessCombineMode();

		void SetFrictionCombineMode(CombineMode mode);
		void SetBouncinessCombineMode(CombineMode mode);

		ASSET_CLASS_TYPE(PhysicsMaterial);

		void* GetRuntimeBody()const { return m_RuntimeBody; }
	private:
		void* m_RuntimeBody = nullptr;
		friend class PhysicsEngine;
		friend class PhysicsActor;
	};
}