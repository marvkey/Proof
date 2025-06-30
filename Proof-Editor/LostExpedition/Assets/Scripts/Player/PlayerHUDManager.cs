
using System;
using System.Runtime.InteropServices;
using Proof;

namespace LostExpedition
{
    public class PlayerHUDManager : Entity
    {
        PlayerHUDComponent m_PlayerHUDComponent;
        Inventory m_Inventory;

        public UIPanel GunPanel;
        public UIPanel PlayerDefaultHUD;

        public Texture2D EmptyInventory;
        void OnCreate()
        {
            m_PlayerHUDComponent = GetComponent<PlayerHUDComponent>();
            if (m_PlayerHUDComponent != null)
                m_PlayerHUDComponent.PushPanel("HUD",PlayerDefaultHUD,true);
        }

        void OnUpdate(float deltaTime)
        {
            if (m_PlayerHUDComponent == null)
                return;
            
            if (m_Inventory == null)
            {
                m_Inventory = GetScriptInstance<Inventory>();
                if (m_Inventory != null)
                {
                    m_Inventory.OnCurrentItemChanged += InventoryItemChange;
                    m_Inventory.OnInventoryItemsChange += InvenoryItemAddedorRemoved;

                }
            }

            if(m_Inventory != null)
            {
                InventoryItem item = m_Inventory.GetCurrentItem();
                if (item == null) return;
                if (item.HasScriptInstance<Gun>())
                    UpdateGunHudPanel(item as Gun);
            }
        }

        void InventoryItemChange(InventoryItem currentItem, InventoryItem previousItem)
        {
            if (GunPanel == null)
                return;
            if (previousItem != null)
            {
                m_PlayerHUDComponent.RemovePanel("HUD", GunPanel);
            }
            m_PlayerHUDComponent.PushPanel("HUD", GunPanel, true);

        }

        void UpdateCurrentInventoryHud()
        {
            if (m_Inventory == null)
                return;

           InventoryItem item =  m_Inventory.GetCurrentItem();
            if (item == null) return;
            if (item.HasScriptInstance<Gun>())
                UpdateGunHudPanel(item as Gun);
        }

        void UpdateGunHudPanel(Gun gun)
        {
            Variable Ammo = m_PlayerHUDComponent.GetRegistryVariable("HUD", GunPanel, "Ammo");
            if (Ammo != null)
                Ammo.SetData(gun.GetAmmo().ToString());

            Variable magazineSize = m_PlayerHUDComponent.GetRegistryVariable("HUD", GunPanel, "MagazineSize");
            if (magazineSize != null)
                magazineSize.SetData(gun.MagazineSize.ToString());
        }

        Variable GetInventoryImageSlot(int slot)
        {
            switch(slot)
            {
                case 0:
                    return m_PlayerHUDComponent.GetRegistryVariable("HUD", PlayerDefaultHUD, "Invenotory0");
                    break;
                case 1:
                    return m_PlayerHUDComponent.GetRegistryVariable("HUD", PlayerDefaultHUD, "Invenotry1");
                    break;
                case 2:
                    return m_PlayerHUDComponent.GetRegistryVariable("HUD", PlayerDefaultHUD, "Inventory2");
                    break;

            }

            return null;
        }
        void InvenoryItemAddedorRemoved(InventoryItem currentItem, InventoryItem previousItem,int slot, bool added)
        {
            if(added)
            {
                Variable var = GetInventoryImageSlot(slot);
                var.SetData(currentItem.InventoryImage);
            }
        }
    }
}
