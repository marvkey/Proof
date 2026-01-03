
using System;
using Proof;

namespace NullState
{
	public class DamageComponent : Entity
	{
		[ShowInEditorAttribute("Damage Amount")]
        float DamageAmount = 10.0f;

        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{
		}

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
		}

		// OnPhysicsUpdate is called at a fixed timestep for physics-related logic
		void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
		{
		}

		public void DealDamage(Entity other)
		{
			var healthComp = other.GetScript<HealthComponent>();

			if(healthComp != null)
			{ 
				healthComp.TakeDamage(this, DamageAmount);
			}
        } 
    }
}
