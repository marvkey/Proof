
using System;
using Proof;

namespace LostExpedition
{
    public class HealthComponent : Entity
    {
        public event Action<Entity,float> OnTakeDamage;          // Called with damage amount
        public event Action OnHealthDepleted;             // Called when health <= 0

        [ShowInEditorAttribute("Max Health")]
        float MaxHealth = 100.0f;
        [ShowInEditorAttribute("Health")]
        float Health = 100.0f;

        public bool BlockTakeDamage = false; // If true, will not take damage

        public bool UseHunger = false;

        [ShowInEditorAttribute("Max Hunger")]
        float MaxHunger = 100.0f;
        float m_CurrentHunger = 0.0f;
        public float DecreaseHungerPerSecond = 0.5f; 
        public float HungerDecreaseHealthPerSeond = 1.0f; // how much health shoudl be removed if plaeyr starving

        public float GetHealth()
        {
            return Health;
        }

        public float GetMaxHealth()
        {
            return MaxHealth;
        }

        public float GetMaxHunger()
        {
            return MaxHunger;
        }
        public float GetCurrentHunger()
        {
            return m_CurrentHunger;
        }

        public void AddHunger(float hunger)
        {
            m_CurrentHunger += hunger;
            m_CurrentHunger = Mathf.Clamp(m_CurrentHunger, 0.0f, MaxHunger);

            Log.Trace($"{Name} added hunger {hunger} current hunger {m_CurrentHunger}");
        }

        public bool DestoryIfHealthZero = true;
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
        {
            m_CurrentHunger = MaxHunger /2;
        }

        void OnUpdate(float dt)
        {
            OnUpdateHunger(dt);
        }

        void OnUpdateHunger(float dt)
        {
            if (UseHunger == false) { return; }
            m_CurrentHunger -= DecreaseHungerPerSecond * dt;

            m_CurrentHunger = Mathf.Clamp(m_CurrentHunger, 0.0f, MaxHunger);

            if (m_CurrentHunger <= 0.0f)
                TakeDamage(this, HungerDecreaseHealthPerSeond * dt);

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

        protected virtual void OnPostUpdate(float ts) 
        {
            if (Health <= 0.0f && DestoryIfHealthZero)
                Destroy();
        }

    }
}
