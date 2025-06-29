using System;
using System.Runtime.InteropServices.WindowsRuntime;
using Proof;


namespace LostExpedition
{
    public class InventoryItem : Entity
    {
        public Texture2D InventoryImage;

        protected Inventory m_OwnerInventory = null;
        public virtual void SetActiveInventory()
        {

        }
        public virtual void SetDeactiveInventory()
        {

        }

        public  void PickUp(Inventory inventory)
        {
            GetComponent<MeshComponent>().Visible = false;
            RemoveComponent<RigidBodyComponent>();

            m_OwnerInventory = inventory;
        }
        public void Drop()
        {
            GetComponent<MeshComponent>().Visible = true;
            AddComponent<RigidBodyComponent>();

            m_OwnerInventory = null;
        }
    }
    public class Inventory : Entity
    {
        public int NumItemSlots = 3;

        private int m_CurrentSlot = 0;

        private InventoryItem[] InventoryItems;
        public Entity InventoryHandleSlot;

        
        void OnCreate()
        {
            InventoryItems = new InventoryItem[NumItemSlots];

            TriggerEnterEvent += OnTriggerItemEnter;

        }

        void OnTriggerItemEnter(Entity item)
        {
            Log.Info("Item enter");

            if (item.HasScriptInstance<InventoryItem>())
            {
                 AddItem(item.GetScriptInstance<InventoryItem>());
            }


        }
        public void AddItem(InventoryItem item)
        {
            int slot = FindFirstAvailableSlot();
            if(slot != -1)
            {
                InventoryItems[slot] = item;
            }

            AddChild(item);

            item.RemoveComponent<RigidBodyComponent>();
            // changwe everythign but the scale
            Transform finalLocalTransform = new Proof.Transform();
            finalLocalTransform.Location = InventoryHandleSlot.Transform.Location;
            finalLocalTransform.Rotation = InventoryHandleSlot.Transform.Rotation;

            finalLocalTransform.Scale = item.Transform.LocalTransform.Scale;

            item.Transform.Location = finalLocalTransform.Location;
            item.Transform.Rotation = finalLocalTransform.Rotation;
            item.PickUp(this);

            if (slot == m_CurrentSlot)
                item.SetActiveInventory();

            Log.Info($"{Name} Picked up Item: {item.Name} Invenotry SLot {slot}");

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

            InventoryItem previousItem = GetCurrentItem();

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


            if (previousItem != null)
                previousItem.SetDeactiveInventory();


            InventoryItem newItem = GetCurrentItem();
            newItem.SetActiveInventory();
        }

        public void DecreaseCurrentSlot()
        {
            int slot = m_CurrentSlot;
            InventoryItem previousItem = GetCurrentItem();

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

            if (previousItem != null)
                previousItem.SetDeactiveInventory();


            InventoryItem newItem = GetCurrentItem();
            newItem.SetActiveInventory(); ;
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
