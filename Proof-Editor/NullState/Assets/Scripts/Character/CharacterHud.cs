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

        }
    }
}
