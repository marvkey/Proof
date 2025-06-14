
using System;
using Proof;

namespace LostExpedition
{
    public class HealthComponent : Entity
    {
        [ShowInEditorAttribute("Max Health")]
        float MaxHealth = 100.0f;
        [ShowInEditorAttribute("Health")]
        float Health = 100.0f;


        public float GetHealth()
        {
            return Health;
        }

        public float GetMaxHealth()
        {
            return MaxHealth;
        }
        public bool DestoryIfHealthZero = true;
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
        {
        }

        public void TakeDamage(float damage)
        {
            Health -= damage;
            Log.Info($"{Name} took {damage} damage, Health: {Health}");
           
        }

        protected virtual void OnPostUpdate(float ts) 
        {
            if (Health <= 0.0f && DestoryIfHealthZero)
                Destroy();
        }

    }
}
