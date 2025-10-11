using System;
using Proof;

namespace NullState
{
    public class CharacterHud : Entity
    {
        public UIPanel PlayerDefaultHUD;
        PlayerHUDComponent m_HUDComponent;

        void OnCreate()
        {
            m_HUDComponent = GetComponent<PlayerHUDComponent>();
        }

        void OnUpdate(float deltaTime)
        {
            if(m_HUDComponent ==null)
            {
                Log.Error($"{Name} CharacterHud Script needs a PlayerHUDComponent");
                return;
            }
            ApplyDefaultHUD();
            
        }

        void ApplyDefaultHUD()
        { 
            if (!HasScript<EntropyCompnent>()) return;

            EntropyCompnent entropyCompnent = GetScript<EntropyCompnent>();

            float entropyPercent = entropyCompnent.EntropyPercent;
            Variable entropyPercentVar = m_HUDComponent.GetRegistryVariable("HUD", PlayerDefaultHUD, "EntropyPercent");

            if(entropyPercentVar != null)
                entropyPercentVar.SetData(entropyPercent);

            Variable entropyVar = m_HUDComponent.GetRegistryVariable("HUD", PlayerDefaultHUD, "Entropy");
            if(entropyVar != null)
            {
                entropyVar.SetData((string)entropyCompnent.Entropy.ToString("0"));

            }

            Variable speedVar = m_HUDComponent.GetRegistryVariable("HUD", PlayerDefaultHUD, "PlayerSpeed");
            if (speedVar != null)
            {
                speedVar.SetData((string)GetScript<CharacterMovement>().GetCurrentSpeed().ToString("0"));
            }

            Variable healthVar = m_HUDComponent.GetRegistryVariable("HUD", PlayerDefaultHUD, "Heatlh");

            if (healthVar != null)
            {
                HealthComponent healthComponent;
                if (!HasScript<HealthComponent>())
                    return;
                healthComponent = GetScript<HealthComponent>();
                float health = healthComponent.HealthPercentage;

                healthVar.SetData(health.ToString("F1") + "%");

            }
        }
    }
}
