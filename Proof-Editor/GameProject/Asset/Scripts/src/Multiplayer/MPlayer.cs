using Proof;
using ProofScriptCore.src.Proof.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Game
{

    public class MPlayer : Entity
    {
        public float FowardForce = 1000f;
        private float m_SlowSpeed;
        private RigidBodyComponent m_RigidBody;
        PowerUp[] m_Inventory = new PowerUp[2];
        PlayerHUDComponent m_HUD;
        private int m_CurrentInventory = 0;
        public float SideWayForce = 200f;
        public bool DisableFowardForce = false;
        void OnCreate()
        {
            m_RigidBody = GetComponent<RigidBodyComponent>();
            m_HUD = GetComponent<PlayerHUDComponent>();
            SetMotion("MoveX", MoveX);
            m_Inventory[0] = null;
            m_Inventory[1] = null;
            //SetMotion("RMouyseotateX",)
            //SetMotion("RMouyseotateY",)

            SetAction("ChangeSpeed", InputState.KeyClicked, SlowSpeed);
            SetAction("ChangeSpeed", InputState.KeyReleased, BackToNormalSpeed);
            SetAction("Jump", InputState.KeyClicked, Jump);

            SetAction("IncreaseInventory", InputState.KeyClicked, InCreaseInventory);
            SetAction("DecreaseInventory", InputState.KeyClicked, DecreaseInventory);
            SetAction("Drop", InputState.KeyClicked, Drop);
            SetAction("ApplyPowerUp", InputState.KeyClicked, ApplyPowerUp);
            m_SlowSpeed = FowardForce / 2;
        }

        void OnUpdate(float ts)
        {
            UIPanel viewPanel = m_HUD.GetPanel(0);
            UIButtonImage slot0 = viewPanel.GetImageButton("Inventory0");
            Log.Info("Color of slot "+slot0.Color);
            if (m_Inventory[0] != null)
            {
                slot0.Image = m_Inventory[0].Image;
            }
            //slot0.Color = new Vector4( Proof.Random.Float(0, 1), Proof.Random.Float(0, 1), Proof.Random.Float(0, 1),1);
            if (m_RigidBody == null || DisableFowardForce == true) return;
            m_RigidBody.AddForce(GetComponent<TransformComponent>().GetFowardVector()* FowardForce * ts);
        }
        void Pressed()
        {
            Log.Error("Pressed ");
        }

        void MoveX(float motionValue)
        {
            m_RigidBody.AddForce(new Vector(SideWayForce * World.GetTimeStep()* motionValue, 0, 0), ForceMode.VelocityChange);
        }

        void SlowSpeed()
        {
            FowardForce /= 2;
            SideWayForce /= 2;
            Log.Info("Slow Speed");
        }
        void BackToNormalSpeed()
        {
            FowardForce *= 2;
            SideWayForce *= 2;
            Log.Info("Normal Speed");

        }
        void Jump()
        {
            m_RigidBody.AddForce(new Vector(0, 10,0), ForceMode.Impule);
            Log.Info("jump");
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
