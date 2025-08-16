using System;
using System.Security.Principal;
using Proof;

namespace LostExpedition
{

    class MeleeWeaponHitBox : Entity
    {
        MeleeWeapon m_Weapon = null;
        public void SetOwnerMeleWeapon(MeleeWeapon weapon)
        {
            m_Weapon = weapon;
            TriggerEnterEvent += OnTriggerHitBoxEnter;
        }

        public void OnTriggerHitBoxEnter(Entity hitEntity)
        {
            if (hitEntity == null) return;
            m_Weapon.OnTriggerHitBoxEnter(hitEntity);
        }
    }
    class MeleeWeapon : Weapon
    {
        public float SwingDamage = 5;
        public float SwingCooldown = 1.5f;
        public float HitRange = 3;


        private float m_CooldownTimer = 0f;
        private bool m_IsSwinging = false;

        // Swing visual state
        private float m_SwingTimer = 0f;
        private bool m_IsReturningToPositionSwing = false;
        private Quaternion m_StartRot;
        private Quaternion m_EndRot;


        public Entity HitBox;
        private Entity m_EntityCallingSwing;

        void OnCreate()
        {
            // Initialize melee weapon properties

            if (HitBox != null && HitBox.HasScript<MeleeWeaponHitBox>())
            {
                HitBox.GetScriptInstance<MeleeWeaponHitBox>().SetOwnerMeleWeapon(this);
            }
        }

        void OnUpdate(float dt)
        {
            if (m_CooldownTimer > 0f)
                m_CooldownTimer -= dt;

            // Swing forward
            if (m_IsSwinging)
            {
                m_SwingTimer += dt;
                float t = Math.Min(m_SwingTimer / SwingCooldown, 1f);
                Transform.RotationQuat = Quaternion.SLerp(m_StartRot, m_EndRot, t);

                if (t >= 1f)
                {
                    m_IsSwinging = false;
                    m_IsReturningToPositionSwing = true;
                    m_SwingTimer = 0f; // reset for return phase
                }
            }
            // Return to start
            else if (m_IsReturningToPositionSwing)
            {
                m_SwingTimer += dt;
                float t = Math.Min(m_SwingTimer / (SwingCooldown/2), 1f);
                Transform.RotationQuat = Quaternion.SLerp(m_EndRot, m_StartRot, t);

                if (t >= 1f)
                {
                    m_IsReturningToPositionSwing = false;
                    Transform.RotationQuat = m_StartRot; // ensure exact reset

                }
            }
            else
            {
                m_EntityCallingSwing = null;
            }


        }

        public void OnTriggerHitBoxEnter(Entity hitEntity)
        {
            if (m_IsSwinging == false) return;
             
            if (hitEntity != null && hitEntity.HasScript<HealthComponent>() && hitEntity != m_EntityCallingSwing)
            {
                var health = hitEntity.GetScript<HealthComponent>();
                health.TakeDamage(m_EntityCallingSwing, SwingDamage);

            }

        }
        public override void Activate(Entity player, TransformComponent playerTransform, TransformComponent cameraTransform)
        {

            if (m_CooldownTimer > 0f || m_IsSwinging || m_IsReturningToPositionSwing)
                return;

            m_CooldownTimer = SwingCooldown;
            m_EntityCallingSwing = player;
            // Record starting rotation
            m_StartRot = Transform.RotationQuat;

            // Define target swing rotation (change values to tweak style)
            Quaternion swingOffset = Quaternion.EularToQuat(new Vector3(-90f * Mathf.Deg2Rad, 0.0f, 0f)); // 90° yaw swing
            m_EndRot = m_StartRot * swingOffset;

            m_IsSwinging = true;
            m_SwingTimer = 0f;

            // Do raycast hit logic immediately
        }


        private void DoHit(Entity player, TransformComponent playerTransform, TransformComponent cameraTransform)
        {
            Vector3 origin = playerTransform.Location;
            Vector3 direction = cameraTransform.Forward.Normalized;

            RaycastData data = new RaycastData();
            data.Origin = origin;
            data.Direction = direction;
            data.MaxDistance = HitRange;
            data.ExcludedEntities = new ulong[3];
            data.ExcludedEntities[0] = GetParent().ID; // Exclude the parent (player)
            data.ExcludedEntities[1] = ID;             // Exclude the weapon itself
            data.ExcludedEntities[1] = HitBox.ID;             // Exclude the weapon itself

            if (Physics.RayCast(data, out RaycastHit hit))
            {
                var hitEntity = hit.Entity;
                if (hitEntity != null && hitEntity.HasScript<HealthComponent>())
                {
                    var health = hitEntity.GetScript<HealthComponent>();
                    health.TakeDamage(player,SwingDamage);

                    Log.Warn($"{hitEntity.Name} got hit by {HitBox.Name}");
                }
            }

        }
    }
}
