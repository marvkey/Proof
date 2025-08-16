using System;
using System.Runtime.InteropServices;
using Proof;

namespace LostExpedition
{
    class Food : InventoryItem
    {
        float HungerRestore = 25.0f;

        public void OnCreate()
        {
            Log.Trace($"{Name} spawned hunger resore {HungerRestore}");
        }
        public override void Activate(Entity player, TransformComponent playerTransform, TransformComponent cameraTransform)
        {
            if (player == null)
                return;

            if (!player.HasScript<HealthComponent>()) return;
                player.GetScript<HealthComponent>().AddHunger(HungerRestore);

            if (m_OwnerInventory!= null)
                m_OwnerInventory.RemoveItem(this);

            Log.Trace($"Food: {Name} eaten");

            Destroy();
        }

    }
}
