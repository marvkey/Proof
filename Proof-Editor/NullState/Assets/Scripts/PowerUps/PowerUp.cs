using Proof;

namespace NullState
{
    public enum PowerUpState
    {
        Ready,
        Active,
        Recharging,
        Empty,
    };

    public enum PowerUpUsageType
    {
        Continous,
        Discrete
    }

    public enum PowerUpRechargeMode
    {
        Immediately,
        WhenEmpty,
    }

    public abstract class PowerUp : Entity
    {
        [ShowInEditorAttribute("State", true)]
        protected PowerUpState m_State = PowerUpState.Ready;

        [ShowInEditorAttribute("Charge", true)]
        protected float m_Charge;

        [Seperator]
        [ShowInEditorAttribute("UsageType", true)]
        protected PowerUpUsageType m_UsageType;

        public float MaxCharge = 1.0f;

        [Seperator]
        public PowerUpRechargeMode ChargingMode = PowerUpRechargeMode.Immediately;

        [ToolTip("how fast we charge (per second)")]
        public float ChargeAmount = 0.1f;

        [ToolTip("seconds to wait before charging starts")]
        public float ChargeDelay = 0.0f;

        private float m_RechargeDelayTimer = 0.0f;

        protected virtual void OnCreate()
        {
            m_Charge = MaxCharge;
            m_State = (m_Charge > 0.0f) ? PowerUpState.Ready : PowerUpState.Empty;
        }

        protected virtual void OnUpdate(float ts)
        {
            if (m_RechargeDelayTimer > 0.0f)
                m_RechargeDelayTimer = Mathf.Max(0.0f, m_RechargeDelayTimer - ts);

            UpdatePowerup(ts);

            UpdateRecharge(ts);
        }

        protected void StartRechargeDelay()
        {
            m_RechargeDelayTimer = ChargeDelay;
        }

        protected bool CanRecharge()
        {
            if (m_RechargeDelayTimer > 0.0f)
                return false;

            if (m_Charge >= MaxCharge)
                return false;

            if (ChargingMode == PowerUpRechargeMode.WhenEmpty)
                return m_Charge <= 0.0f;

            return true; // Immediately
        }

        private void UpdateRecharge(float dt)
        {
            if (!CanRecharge())
                return;

            m_Charge += ChargeAmount * dt;
            m_Charge = Mathf.Min(m_Charge, MaxCharge);

            // set state based on charge (child classes may override state when active)
            if (m_Charge <= 0.0f)
                m_State = PowerUpState.Empty;
            else if (m_Charge < MaxCharge)
                m_State = PowerUpState.Recharging;
            else
                m_State = PowerUpState.Ready;
        }

        protected void Consume(float amount)
        {
            m_Charge -= amount;
            m_Charge = Mathf.Max(m_Charge, 0.0f);

            if (m_Charge <= 0.0f)
            {
                m_State = PowerUpState.Empty;
                StartRechargeDelay();
            }
            else
            {
                // if we consumed but still have charge, we might want to start recharging (Immediately mode)
                if (ChargingMode == PowerUpRechargeMode.Immediately)
                    StartRechargeDelay();
            }
        }

        protected abstract void UpdatePowerup(float dt);
    }

    // Continuous: enable/disable, drains per second while enabled
    public abstract class ContinousPowerUp : PowerUp
    {
        [Seperator]
        [ToolTip("drain per second while enabled")]
        public float DrainAmount = 0.25f;

        [ShowInEditorAttribute(false)]
        protected bool m_IsEnabled = false;

        protected ContinousPowerUp()
        {
            m_UsageType = PowerUpUsageType.Continous;
        }

        // public-ish API for your player/controller code
        public void EnablePower()
        {
            if (m_IsEnabled)
                return;

            if (m_Charge <= 0.0f)
            {
                m_State = PowerUpState.Empty;
                return;
            }

            m_IsEnabled = true;
            m_State = PowerUpState.Active;
            OnEnabled();
        }

        public void DisablePower(bool forced = false)
        {
            if (!m_IsEnabled)
                return;

            m_IsEnabled = false;

            // when you turn it off, start recharge delay
            StartRechargeDelay();

            // state resolves during recharge update
            if (m_Charge <= 0.0f)
                m_State = PowerUpState.Empty;
            else
                m_State = PowerUpState.Recharging;

            OnDisabled(forced);
        }

        public bool CanEnable()
        {
            return m_Charge > 0.0f;
        }

        protected override void UpdatePowerup(float dt)
        {
            if (!m_IsEnabled)
                return;

            m_State = PowerUpState.Active;

            // drain per second
            m_Charge -= DrainAmount * dt;
            if (m_Charge <= 0.0f)
            {
                m_Charge = 0.0f;
                m_State = PowerUpState.Empty;

                // forced disable
                m_IsEnabled = false;
                OnDisabled(true);

                StartRechargeDelay();
                return;
            }

            OnWhileEnabled(dt);
        }

        // hooks
        protected virtual void OnEnabled() { }
        protected virtual void OnDisabled(bool forced) { }
        protected virtual void OnWhileEnabled(float dt) { }
    }

    // Discrete: fire consumes charge (shots), no enable/disable
    public abstract class DiscretePowerUp : PowerUp
    {
        [Seperator]
        [ToolTip("drain per use (shot cost)")]
        public float DrainAmount = 1.0f;

        protected DiscretePowerUp()
        {
            m_UsageType = PowerUpUsageType.Discrete;
        }

        public bool CanFire()
        {
            return m_Charge >= DrainAmount;
        }

        public void Fire()
        {
            if (!CanFire())
            {
                m_State = PowerUpState.Empty;
                StartRechargeDelay();
                return;
            }

            // consume first (so your fire logic can read new remaining charge if needed)
            Consume(DrainAmount);

            // IMPORTANT: WhenEmpty vs Immediately behavior:
            // - Consume() starts recharge delay if we hit 0
            // - for Immediately mode, we want recharge to start even if not empty
            if (ChargingMode == PowerUpRechargeMode.Immediately && m_Charge > 0.0f)
                StartRechargeDelay();

            OnFire();
        }

        protected override void UpdatePowerup(float dt)
        {
            // Discrete has no per-frame drain; recharge handled in base.
        }

        protected abstract void OnFire();
    }
}
