
using System;
using Proof;

namespace NullState
{
	public class HealthComponent : Entity
	{
        [ShowInEditorAttribute("Max Health")]
        float MaxHealth = 100.0f;
        [ShowInEditorAttribute("Health")]
        float Health = 100.0f;

        public event Action<Entity, float> OnTakeDamage;          // Called with damage amount
        public event Action OnHealthDepleted;             // Called when health <= 0

        [Seperator]
        public bool BlockTakeDamage = false; // If true, will not take damage
        public bool DestoryIfHealthZero = true;

        public float GetHealth()
        {
            return Health;
        }

        public float GetMaxHealth()
        {
            return MaxHealth;
        }

        public float HealthPercentage => (GetHealth() / GetMaxHealth())*100;

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


        public void TakeDamage(Entity other, float damage)
        {
            OnTakeDamage?.Invoke(other, damage);

            if (BlockTakeDamage)
                return;

            Health -= damage;
            Log.Info($"{Name} took {damage} damage, Health: {Health}");



            if (Health <= 0.0f)
            {
                OnHealthDepleted?.Invoke();

                if (DestoryIfHealthZero)
                    Destroy();
            }
        }
    }
}
