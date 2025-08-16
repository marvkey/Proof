
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices.WindowsRuntime;
using Proof;

namespace LostExpedition
{
    public class TrapTreePlayerSettings
    {
        public float CurrentThreshold;
        public bool BrokeThreshold;

        public TrapTreePlayerSettings(float currentThreshold, bool brokeThreshold)
        {
            CurrentThreshold = currentThreshold;
            BrokeThreshold = brokeThreshold;
        }
        public TrapTreePlayerSettings()
        {
            CurrentThreshold = 0.0f;
            BrokeThreshold = false;
        }
    }
    public class TrapTreeManager : Entity
    {

        public float ThresholdSeconds = 15;
        public float AmountPerKilledTree = 5;
        public float DamagePerTree = 2; // How much damage the tree does to the player when they trigger it)

        // Per-player cooldown timer
        Dictionary<Entity, TrapTreePlayerSettings> PlayerCooldowns;

        public void OnCreate()
        {

            PlayerCooldowns = new Dictionary<Entity, TrapTreePlayerSettings>();
        }

        public bool PlayerBrokeCooldown(Entity player)
        {
            if (!PlayerCooldowns.ContainsKey(player))
            {
                PlayerCooldowns[player] = new TrapTreePlayerSettings(0.0f, false);
            }

            return PlayerCooldowns[player].BrokeThreshold;
        }
        public void ResetPlayerTheshold(Entity player)
        {
            PlayerCooldowns[player] = new TrapTreePlayerSettings(0.0f, false);
        }


        public void PlayerAppliedDamage(TrapTree tree, Entity player)
        {
            if(!PlayerCooldowns.ContainsKey(player))
            {
                PlayerCooldowns[player] = new TrapTreePlayerSettings(0.0f, false);
            }

            if(PlayerCooldowns[player].BrokeThreshold)
            {
                Log.Warn($"{player.Name} has already broken the threshold and cannot trigger the trap tree again until the cooldown resets.");

                PlayerCooldowns[player].CurrentThreshold = 0.0f; // Reset the cooldown timer
                return;
            }
            else
            {
                PlayerCooldowns[player].CurrentThreshold += DamagePerTree;
            }
        }


        public void PlayerKilledTree(TrapTree tree, Entity player)
        {
            if (PlayerCooldowns.ContainsKey(player))
            {
                PlayerCooldowns[player].CurrentThreshold += AmountPerKilledTree;
                Log.Trace($"{player.Name} killed tree {tree.ID}");
                Log.Trace($"{player.Name} current trshold {PlayerCooldowns[player].CurrentThreshold}");
                Log.Warn($"{player.Name} has {PlayerCooldowns[player].CurrentThreshold} seconds of cooldown left before they can trigger a trap tree again.");
            }
        }


        public void OnUpdate(float dt)
        {

            foreach (var player in PlayerCooldowns)
            {

                if (player.Value.CurrentThreshold > ThresholdSeconds)
                {
                    player.Value.BrokeThreshold = true;
                }
                player.Value.CurrentThreshold -= dt;
                if(player.Value.CurrentThreshold < 0)
                    player.Value.CurrentThreshold = 0;

            }

        }

    }


    public class TrapTree : Entity
    {
        HealthComponent m_HealthComponent;
        TrapTreeManager m_TrapTreeManager;
        Vector3 m_InitialScale;
        Vector3 m_InitialLocation;
        public Prefab SpawnOnDelete;

        Entity m_LastDamagedPlayer;

        public float RiseHeight = 4f;
        public float RiseDuration = 3.0f;
        public float Damage = 20f;

        private bool m_AnimationTriggered = false;
        private float m_AnimationTimer = 0.0f;
        float m_TotalAnimationTime = 2.5f;
        float m_PauseDuration = 0.4f;
        bool m_HasRisen = false;
        bool m_HasCrushed = false;
        Vector3 m_RiseTarget;
        Vector3 m_AnimationInitalLocation;

        public void OnCreate()
        {
            m_InitialScale = GetParent().Scale;
            m_InitialLocation = GetParent().Location;
            if(!HasScript<HealthComponent>())
            {
                Log.Error($"{Name} needs a HealthComponent");
                return;
            }

            m_HealthComponent = GetScript<HealthComponent>();

            m_HealthComponent.OnTakeDamage += OnHealthComponentTakeDamage;

            m_TrapTreeManager = World.TryFindEntityByTag("TrapTreeManager")?.GetScript<TrapTreeManager>();

            if(m_TrapTreeManager == null)
            {
                Log.Error($"{Name} could not find TrapTreeManager in the world");
                return;
            }
        }
        private void OnUpdate(float dt)
        {
            PlayTrapAnimation(dt);
        }
        public void OnTriggerEnter(Entity entity)
        {
            if (m_HealthComponent == null)
                return;

            if (m_TrapTreeManager == null)
                return;

        }
        protected override void OnDestroy() 
        {
            
            if(SpawnOnDelete != null && SpawnOnDelete.ID.IsValid())
            {
                World.Instantiate(SpawnOnDelete, m_InitialLocation);
                
            }

            if (m_TrapTreeManager != null)
                m_TrapTreeManager.PlayerKilledTree(this, m_LastDamagedPlayer);
        }

        void OnHealthComponentTakeDamage(Entity player, float damage) 
        {
            
            if (m_TrapTreeManager == null)
                return;

            if (m_AnimationTriggered) return;
            m_LastDamagedPlayer = player;


            if (m_TrapTreeManager.PlayerBrokeCooldown(player))
            {
                m_HealthComponent.BlockTakeDamage = true;
                m_AnimationInitalLocation = GetParent().Location;
                m_AnimationTriggered = true;
                Log.Warn("Player broke threshol");

                return;
            }
            else
            {
                m_HealthComponent.BlockTakeDamage = false;
            }


            Log.Warn($"{Name} taken from {player.Name}");

            m_TrapTreeManager.PlayerAppliedDamage(this, player);

            float initialScaleLength = m_InitialScale.Length;

            // percentage of scale of the vecotor so if damage is 10, and inital scale is 100
            // (10/100) * 100  = 10, so thats 10% of 100
            float removeScale = (damage / 100) * initialScaleLength;

            GetParent().Scale -= (removeScale/2); // the reason dive by 2 dont want ti to be to small

        }

        void PlayTrapAnimation(float dt)
        {

            if (m_AnimationTriggered == false)
                return;

            m_AnimationTimer += dt;

            float riseT = Math.Min(m_AnimationTimer / RiseDuration, 1f);
            if (!m_HasRisen)
            {
                Log.Trace("Rising");
                // Phase 1: Rise straight up
                Vector3 riseTarget = m_AnimationInitalLocation + new Vector3(0, 25, 0);
                GetParent().Location = Vector3.Lerp(m_AnimationInitalLocation, riseTarget, riseT);

                if (riseT >= 1f)
                {
                    m_HasRisen = true;
                    m_AnimationTimer = 0f; // Reset timer for slam phase
                    m_RiseTarget = riseTarget;
                }

                return;
            }

            if (m_HasRisen && !m_HasCrushed)
            {
                if (m_AnimationTimer < m_PauseDuration)
                    return; // Optional delay at the top
                 
                float slamT = Math.Min(m_AnimationTimer / (RiseDuration/2), 1f);

                Vector3 playerPos = m_LastDamagedPlayer.Location;

                Log.Trace(playerPos);
                GetParent().Location = Vector3.Lerp(m_RiseTarget, playerPos, slamT);

                if (slamT >= 1f)
                {
                    m_HasCrushed = true;

                    if (m_LastDamagedPlayer.HasScript<HealthComponent>())
                    {
                        m_LastDamagedPlayer.GetScript<HealthComponent>().TakeDamage(this, Damage);
                        m_TrapTreeManager.ResetPlayerTheshold(m_LastDamagedPlayer);
                        Log.Warn("Player slammed");
                        //Destroy();
                    }
                }
            }
        }
    }
}
