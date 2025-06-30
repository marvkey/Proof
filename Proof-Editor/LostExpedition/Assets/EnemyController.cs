using System;
using System.Runtime.InteropServices.WindowsRuntime;
using Proof;


namespace LostExpedition 
{
    class EnemyController : Entity
    {
       public Entity TextEntityForHealth;

        void OnUpdate(float deltaTime)
        {
            if (TextEntityForHealth != null)
            {
                TextEntityForHealth.GetComponent<TextComponent>().Text = $"Health: {GetScript<HealthComponent>().GetHealth()}";
            }
        }
    }
}
