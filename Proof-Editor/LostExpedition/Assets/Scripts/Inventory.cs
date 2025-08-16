using System;
using System.Collections.Generic;
using System.Runtime.InteropServices.WindowsRuntime;
using Proof;


namespace LostExpedition
{
    public class InventoryItem : Entity
    {
        public string ItemName;

        public int MaxItemPerSlot = 1;
        public Texture2D InventoryImage;

        protected Inventory m_OwnerInventory = null;
        public virtual void SetActiveInventory()
        {
            GetComponent<MeshComponent>().Visible = true;

        }
        public virtual void SetDeactiveInventory()
        {
            GetComponent<MeshComponent>().Visible = false;

        }

        public void PickUp(Inventory inventory)
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

        public virtual void Activate(Entity player, TransformComponent playerTransform, TransformComponent cameraTransform)
        {

        }
    }

    class InventorySlot
    {
        public List<InventoryItem> Items { get; private set; }
        public int MaxItemPerSlot { get; private set; } // Max stack size

        private InventorySlot()
        {

        }
        public InventorySlot(InventoryItem item0)
        {
            MaxItemPerSlot = item0.MaxItemPerSlot;
            Items.Add(item0);
        }

        public bool AddItem(InventoryItem item)
        {
            if (!CanAddItem(item))
                return false;


            Items.Add(item);
            return true;
        }

        public void DropItem(int amount = 1)
        {
            if (Items.Count == 0) return;

            // Clamp amount so it doesn't exceed what's in the slot
            amount = System.Math.Min(amount, Items.Count);

            // Loop backwards to avoid index issues
            for (int i = amount - 1; i >= 0; i--)
            {
                var itemToDrop = Items[Items.Count - 1]; // get last item
                                                         //  SpawnDroppedItem(itemToDrop); // simulate dropping into world
                Items.RemoveAt(Items.Count - 1);
            }
        }

        public bool HasItem(InventoryItem item)
        {
            return Items.Contains(item);
        }

        public bool CanAddItem(InventoryItem item)
        {
            if (Items[0].Name != item.Name) return false;
            if (IsMaxed()) return false;

            return true;
        }

        public void SetActiveInventory()
        {
            Items[0].SetActiveInventory();
        }
        bool IsMaxed()
        {
            if (MaxItemPerSlot == Items.Count) return true;
            return false;
        }
    }

    public class Inventory : Entity
    {
        public int NumItemSlots = 3;

        private int m_CurrentSlot = 0;

        private InventoryItem[] InventoryItems;
        public Entity InventoryHandleSlot;
        public event Action<InventoryItem,InventoryItem> OnCurrentItemChanged; // current item, previous item


        public event Action<InventoryItem, InventoryItem, int ,bool > OnInventoryItemsChange; // if new item added or removed, item at slot now, item at slot before, invenotry index,(true item added, false item dropped)

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
            OnInventoryItemsChange?.Invoke(item, null, slot, true);
           // item.RemoveComponent<RigidBodyComponent>();
            // changwe everythign but the scale
            Transform finalLocalTransform = new Proof.Transform();
            finalLocalTransform.Location = InventoryHandleSlot.Transform.Location;
            finalLocalTransform.Rotation = InventoryHandleSlot.Transform.Rotation;

            finalLocalTransform.Scale = item.Transform.LocalTransform.Scale;

            item.Transform.Location = finalLocalTransform.Location;
            item.Transform.Rotation = finalLocalTransform.Rotation;
            item.PickUp(this);

            if (slot == m_CurrentSlot)
            {
                item.SetActiveInventory();
                OnCurrentItemChanged?.Invoke(item,null);
            }

            Log.Info($"{Name} Picked up Item: {item.Name} Invenotry SLot {slot}");

        }

        public int GetCurrentSlot() { return m_CurrentSlot; }
        public void RemoveItem(int slot)
        {
            if (InventoryItems[slot] == null) return;

            InventoryItems[slot] = null;
            Log.Trace($"{Name} Removed slot {slot} from inventory");
        }

        public void RemoveItem(InventoryItem item)
        {
            for (int i = 0; i < InventoryItems.Length; i++)
            {
                if (InventoryItems[i] == item) 
                { 
                    RemoveItem(i); 
                }
            }
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

            OnCurrentItemChanged?.Invoke(newItem, previousItem);

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

            OnCurrentItemChanged?.Invoke(newItem, previousItem);

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
