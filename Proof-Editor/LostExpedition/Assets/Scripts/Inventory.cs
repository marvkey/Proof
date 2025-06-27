using System;
using System.Runtime.InteropServices.WindowsRuntime;
using Proof;


namespace LostExpedition
{
    public class InventoryItem : Entity
    {
        public Texture2D InventoryImage;
    }
    public class Inventory : Entity
    {
        [Min(1)]
        public int NumItemSlots = 3;

        private int m_CurrentSlot = 0;

        private InventoryItem[] InventoryItems;


        void OnCreate()
        {
            InventoryItems = new InventoryItem[NumItemSlots];
        }
        public void AddItem(InventoryItem item)
        {
            int slot = FindFirstAvailableSlot();
            if(slot != -1)
            {
                InventoryItems[slot] = item;
            }
        }

        public int GetCurrentSlot() { return m_CurrentSlot; }
        public void RemoveItem(int slot)
        {
            if (InventoryItems[slot] == null) return;

            InventoryItems[slot] = null;
        }

        public InventoryItem GetCurrentItem()
        {
            if (InventoryItems[m_CurrentSlot] ==null) return null;

            return InventoryItems[m_CurrentSlot];
        }

        public void IncreaseCurrentSlot()
        {
            int slot = m_CurrentSlot;

            do
            {
                if (slot == InventoryItems.Length - 1)
                {
                    int availableSlot = FindFirstAvailableSlot();
                    if (availableSlot != -1 && availableSlot != m_CurrentSlot)
                    {
                        m_CurrentSlot = availableSlot;
                        break;
                    }
                    else
                    {
                        slot = -1; // so slot++ becomes 0
                    }
                }

                slot++;

                if (InventoryItems[slot] != null)
                {
                    m_CurrentSlot = slot;
                    break;
                }

            } while (slot != m_CurrentSlot);
        }

        public void DecreaseCurrentSlot()
        {
            int slot = m_CurrentSlot;

            // dont want to check innitaly thats why usign do while 
            do
            {
                if(slot == 0)
                {
                    int availableSlot = FindLastAvailableSlot();
                    if (availableSlot != -1 && availableSlot != m_CurrentSlot)
                    {
                        m_CurrentSlot = availableSlot;
                        break;
                    }
                    else
                    {
                        slot = InventoryItems.Length; // it would be -- next lien of code
                    }
                }
                slot--;

                if (InventoryItems[slot] != null)
                {
                    m_CurrentSlot = slot;
                    break;
                }
                    
            }while(slot != m_CurrentSlot); // if slot ever reaches m_CurrentSlot then 
            
                
        }

        int FindFirstAvailableSlot()
        {
            for (int i = 0; i < InventoryItems.Length; i++)
            {
                if (InventoryItems[i] == null)
                    return i;
            }
            return -1; // No slot available
        }

        int FindLastAvailableSlot()
        {
            for (int i = InventoryItems.Length - 1; i >= 0; i--)
            {
                if (InventoryItems[i] == null)
                    return i;
            }
            return -1; // No slot available
        }

    }
}
