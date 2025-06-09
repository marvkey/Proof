using Proof;
namespace LostExpedition
{
    public class WeaponManager : Entity
    {
        public Weapon CurrentWeapon;

        public Entity WeaponHandleSlot;

        void OnCreate()
        {
            TriggerEnterEvent += OnTriggerItemEnter;
        }
        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
        {
        }

        void OnTriggerItemEnter(Entity item)
        {
            if (CurrentWeapon != null)
                return;

            if (item.HasScript<Gun>())
            {
                var weapon = item.GetScript<Gun>();
                CurrentWeapon = weapon;
                Log.Info($"{Name} Picked up weapon: {CurrentWeapon.Name}");
                AddChild(item);

                weapon.RemoveComponent<RigidBodyComponent>();
                // changwe everythign but the scale
                Transform finalLocalTransform = new Proof.Transform();
                finalLocalTransform.Location = WeaponHandleSlot.Transform.Location;
                finalLocalTransform.Rotation = WeaponHandleSlot.Transform.Rotation;

                finalLocalTransform.Scale = item.Transform.LocalTransform.Scale;

                item.Transform.Location = finalLocalTransform.Location;
                item.Transform.Rotation = finalLocalTransform.Rotation;
            }
        }
    }
}
