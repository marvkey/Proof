using Proof;
using ProofScriptCore.src.Proof.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Game
{

    public class MPlayer : Entity
    {
       // base speed equals 1500
       // slow equals 1000
       // fast equals 2000
        public float FowardForce = 1500f;
        private RigidBodyComponent m_RigidBody;
        PowerUp[] m_Inventory = new PowerUp[2];
        PlayerHUDComponent m_HUD;
        private int m_CurrentInventory = 0;
        public float SideWayForce = 500f;
        public bool DisableFowardForce = false;

        private bool m_IncreaseSpeedChange = false;
        private bool m_DecreaseSpeedChange = false;

        private float m_IncreaseSpeedValue;
        private float m_DecreaseSpeedValue;

        private float m_FowardForceChange = 1000f;

        bool m_Jumping = false;
        bool m_TouchingPlane = false;

        bool m_BlockOnUpdate = true;

        float m_BeginTimer = 5.0f;

        float m_StartCountdown = 1;
        void OnCreate()
        {
            m_RigidBody = GetComponent<RigidBodyComponent>();
            m_HUD = GetComponent<PlayerHUDComponent>();
            SetMotion("MoveX", MoveX);
            SetMotion("IncreaseSpeed", IncreaseSpeed);
            SetMotion("DecreaseSpeed", DecreaseSpeed);
            m_Inventory[0] = null;
            m_Inventory[1] = null;
            //SetMotion("RMouyseotateX",)
            //SetMotion("RMouyseotateY",)

            //SetAction("ChangeSpeed", InputState.KeyReleased, BackToNormalSpeed);
            SetAction("Jump", InputState.KeyClicked, Jump);

            SetAction("IncreaseInventory", InputState.KeyClicked, InCreaseInventory);
            SetAction("DecreaseInventory", InputState.KeyClicked, DecreaseInventory);
            SetAction("Drop", InputState.KeyClicked, Drop);
            SetAction("ApplyPowerUp", InputState.KeyClicked, ApplyPowerUp);
        }
        void Countdown(float ts)
        {
             UIPanel countdownPanel =  m_HUD.GetPanel(1);
            SetInputState(PlayerInputState.Disabled);

            UIText text = countdownPanel.GetText("Countdown");
            int timer = (int)m_BeginTimer;
            text.Text = timer.ToString();
            m_BeginTimer -= ts;

            if(m_BeginTimer < 0)
            {
                SetInputState(PlayerInputState.Enabled);
                m_BlockOnUpdate = false;
                countdownPanel.Visible = false;

            }
           // else
             //   SetInputState(PlayerInputState.Disabled);
        }
        
        void OnUpdate(float ts)
        {
            if (m_BlockOnUpdate)
            {
                m_StartCountdown -= ts;
                if(m_StartCountdown < 0)
                    Countdown(ts);
                return;
            }
            UIPanel viewPanel = m_HUD.GetPanel(0);
            UIButtonImage slot0 = viewPanel.GetImageButton("Inventory0");
            if (m_Inventory[0] != null)
            {
                slot0.Image = m_Inventory[0].Image;
            }
            
            if (m_RigidBody == null || DisableFowardForce == true) return;
            Vector velocity = new Vector(m_RigidBody.GetLinearVelocity().X, m_RigidBody.GetLinearVelocity().Y,FowardForce* ts);
            m_RigidBody.SetLinearVelocity(velocity);
            if(m_IncreaseSpeedChange == true)
            {
                FowardForce -= m_IncreaseSpeedValue;
                m_IncreaseSpeedChange = false;
            }

            if(m_DecreaseSpeedChange == true)
            {
                FowardForce += m_DecreaseSpeedValue;
                m_DecreaseSpeedChange = false;
            }
            if (m_Jumping == true)
            {
                m_RigidBody.SetLinearVelocity(new Vector(m_RigidBody.GetLinearVelocity().X, m_RigidBody.GetLinearVelocity().Y - 8, m_RigidBody.GetLinearVelocity().Z));
            }
        }

        void MoveX(float motionValue)
        {
            Vector velocity = new Vector(SideWayForce*World.GetTimeStep()*motionValue, m_RigidBody.GetLinearVelocity().Y, m_RigidBody.GetLinearVelocity().Z);

           // m_RigidBody.SetLinearVelocity(velocity);

            m_RigidBody.AddForce(new Vector(SideWayForce * World.GetTimeStep()* motionValue, 0, 0), ForceMode.VelocityChange);
        }


        // vaue (0,1)
        void IncreaseSpeed(float motionValue)
        {
            float value = Math.Abs(motionValue);
            m_IncreaseSpeedValue = value * m_FowardForceChange;
            FowardForce  += m_IncreaseSpeedValue;
            m_IncreaseSpeedChange = true;
        }
        void DecreaseSpeed(float motionValue)
        {
            float value = Math.Abs(motionValue);
            m_DecreaseSpeedValue = value * m_FowardForceChange;
            FowardForce -= m_DecreaseSpeedValue;
            m_DecreaseSpeedChange = true;

        }
        void Jump()
        {
            if(m_Jumping)
                return;
            m_Jumping = true;
            m_TouchingPlane = false;
            m_RigidBody.AddForce(new Vector(0, 75,0), ForceMode.Impule);
        }
        void OnCollisionEnter(Entity otherEntity)
        {
            if (otherEntity.Name == "plane")
            {
                if (m_Jumping == true)
                {
                    m_TouchingPlane = true;
                    m_Jumping = false;
                    m_RigidBody.ClearTorque(ForceMode.Impule);
                    m_RigidBody.SetLinearVelocity(new Vector(m_RigidBody.GetLinearVelocity().X, 0, m_RigidBody.GetLinearVelocity().Z));
                    m_RigidBody.SetAngularVelocity(new Vector(0));
                }
                return;
            }
        }
        void ApplyPowerUp()
        {
            if (m_Inventory[m_CurrentInventory] != null)
            {
                m_Inventory[m_CurrentInventory].Throw();
                m_Inventory[m_CurrentInventory] = null;
            }
        }
        void Drop()
        {
            if (m_Inventory[m_CurrentInventory] != null)
            {
                m_Inventory[m_CurrentInventory].Drop();
                m_Inventory[m_CurrentInventory] = null;
                Log.Info("Drop");
            }
        }
        void InCreaseInventory()
        {
            if (m_CurrentInventory == 1)
                m_CurrentInventory = 0;
            else
                m_CurrentInventory++;
            Log.Info("Increase Inventory");
        }

        void DecreaseInventory()
        {
            if (m_CurrentInventory == 0)
                m_CurrentInventory = 1;
            else
                m_CurrentInventory--;
            Log.Info("Decrease Inventory");

        }
        void PickUp(PowerUp powerUp)
        {
            for(int i=0; i<m_Inventory.Length;i++)
            {
                if (m_Inventory[i] == null)
                {
                    m_Inventory[i] = powerUp;
                    powerUp.PickUp(this);
                    break;
                }
            }

        }

        void OnOverllapTriggerEnter(Entity other)
        {
            PowerUp powerUp = other.As<PowerUp>();
            if (powerUp != null)
            {
                if (powerUp.HasOwner())
                    return;
                PickUp(powerUp);
            }
        }
    }
}
