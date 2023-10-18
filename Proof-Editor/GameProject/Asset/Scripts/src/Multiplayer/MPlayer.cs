using Proof;
using System;
using System.Collections.Generic;
using System.Security.AccessControl;

namespace Game
{

    public class MPlayer : Entity
    {
        // camera starting rotation =(0,0,90)
       // base speed equals 1500
       // slow equals 1000
       // fast equals 2000
        public float FowardForce = 1500f;
        private RigidBodyComponent m_RigidBody;
        public bool Finished = false;
        public bool StartFinishCountdown = false;
        float m_FinishCountdown = 5;
        PowerUp[] m_Inventory = new PowerUp[2];
        public ParticleSystemComponent m_ParticleSystem;

        PlayerHUDComponent m_HUD;
        private int m_CurrentInventory = 0;
        public float SideWayForce = 150f;
        public bool DisableFowardForce = false;

        private bool m_IncreaseSpeedChange = false;
        private bool m_DecreaseSpeedChange = false;

        private float m_IncreaseSpeedValue;
        private float m_DecreaseSpeedValue;

        private float m_FowardForceChange = 300f;

        bool m_Jumping = false;
        bool m_TouchingPlane = false;

        bool m_BlockOnUpdate = true;

        float m_BeginTimer = 5.0f;

        float m_StartCountdown = 1;
        private Entity m_TextEntity;
        public int Position = 0;
        private Entity m_CameraEntity;

        float m_MouseSensitivity = 10;
        private List<string> m_LastPlayerMessages;
        bool m_Last = false;
        int m_LastPlayerMessageIndex;
        ParticleSystem[] m_Particles;
        float m_lastPosZ =0;
        float[] m_InvetoryTimers = new float[2];
        void OnCreate()
        {
            Mouse.Capture(true);
            m_CurrentInventory = 0;
         
            m_RigidBody = GetComponent<RigidBodyComponent>();
            m_HUD = GetComponent<PlayerHUDComponent>();
            SetInputState(PlayerInputState.Disabled);

            SetMotion("MoveX", MoveX);
            SetAction("Pause", InputState.KeyClicked,Pause);
            //SetAction("Shutdown", InputState.KeyClicked,Shutdown);
            SetAction("Play", InputState.KeyClicked,Play);
            SetMotion("IncreaseSpeed", IncreaseSpeed);
            SetMotion("DecreaseSpeed", DecreaseSpeed);
            m_Inventory[0] = null;
            m_Inventory[1] = null;
            SetMotion("RMouyseotateX", RMouyseotateX);
            //SetMotion("RMouyseotateY",)

            //SetAction("ChangeSpeed", InputState.KeyReleased, BackToNormalSpeed);
            SetAction("Jump", InputState.KeyClicked, Jump);

            SetAction("IncreaseInventory", InputState.KeyClicked, InCreaseInventory);
            SetAction("DecreaseInventory", InputState.KeyClicked, DecreaseInventory);
            SetAction("Drop", InputState.KeyClicked, Drop);
            SetAction("ApplyPowerUp", InputState.KeyClicked, ApplyPowerUp);

            Entity[] children = GetChildren();
            foreach(Entity child in children)
            {
                if (child.HasComponent<TextComponent>())
                {
                    m_TextEntity = child;
                    break;
                }
            }
            foreach(Entity child in children)
            {
                if(child.HasComponent<ParticleSystemComponent>())
                    m_ParticleSystem = child.GetComponent<ParticleSystemComponent>();
            }
            m_Particles = m_ParticleSystem.GetParticles();

            foreach (Entity child in children)
            {
                if (child.HasComponent<CameraComponent>())
                {
                    m_CameraEntity= child;
                    break;
                }
            }
            m_LastPlayerMessages = new List<string>
            {
                "Imagine being so bad \n your last",
                "Clearly you have \n no freinds",
                "Big man thing \n get good at the game",
                "My blud can't play a game",
                "EMBARASSING!!!!!!!!",
                "Get Help your last"
            };
            m_LastPlayerMessageIndex = Proof.Random.Int(0, m_LastPlayerMessages.Count-1); ;
        }
        void Shutdown()
        {
            if(Mouse.IsMouseCaptured() == false)// basiecaly scene is paused
            {
                Application.Shutdown();
            }
        }
        bool panel0 = false;
        bool panel1 = false;
        bool panel2 = false;
        void Pause()
        {
            Mouse.Capture(false);
            World.Pause();
            panel0 = m_HUD.GetPanel(0).Visible;
            panel1 = m_HUD.GetPanel(1).Visible;
            panel2 = m_HUD.GetPanel(2).Visible;
            m_HUD.GetPanel(0).Visible = false;
            m_HUD.GetPanel(1).Visible = false;
            m_HUD.GetPanel(2).Visible = false;
            m_HUD.GetPanel(3).Visible = true;
        }
  
        void Play()
        {
            Mouse.Capture(true);
            World.Play();
                m_HUD.GetPanel(0).Visible = panel0;
                m_HUD.GetPanel(1).Visible = panel1;
                m_HUD.GetPanel(2).Visible = panel2;

            m_HUD.GetPanel(3).Visible = false;
        }
        void Countdown(float ts)
        {
             UIPanel countdownPanel =  m_HUD.GetPanel(1);
            SetInputState(PlayerInputState.Disabled);

            UIText text = countdownPanel.GetText("Countdown");
            int timer = (int)m_BeginTimer;
            text.Text = timer.ToString();
            m_BeginTimer -= ts;

            if(m_BeginTimer <= 0)
            {
                SetInputState(PlayerInputState.Enabled);
                m_BlockOnUpdate = false;
                countdownPanel.Visible = false;

            }
           // else
             //   SetInputState(PlayerInputState.Disabled);
        }
        public static double ConvertDegreesToRadians(double degrees)
        {
            double radians = (Math.PI / 180) * degrees;
            return (radians);
        }
        // rotaiton sidways
        void RMouyseotateX(float axis)
        {
            if (m_CameraEntity == null) return;
            return;
                Vector3 rotation = GetComponent<TransformComponent>().Rotation;
            rotation.Z += m_MouseSensitivity * World.GetTimeStep() * axis;
          GetComponent<TransformComponent>().Rotation = rotation;

        }
        // original (0,0,90)
        void UpdateCamera(float ts)
        {
            Vector3 rotation = m_CameraEntity.GetComponent<TransformComponent>().Rotation;
            if (rotation.Z > 180)
            {
                rotation.Z = 179;
                m_CameraEntity.GetComponent<TransformComponent>().Rotation = rotation;

            }
            if (rotation.Z < 90)
            {
                rotation.Z = 90;
                m_CameraEntity.GetComponent<TransformComponent>().Rotation = rotation;

            }

            //rotation = GetComponent<TransformComponent>().Rotation;
            //rotation.Y += m_MouseSensitivity * axis * World.GetTimeStep();
            //
            //GetComponent<TransformComponent>().Rotation = rotation;
        }
        private void GameOver(float ts)
        {
            m_HUD.GetPanel(0).Visible = false ;
            m_HUD.GetPanel(1).Visible = false ;
            UIPanel gameOverPanel = m_HUD.GetPanel(2);
            gameOverPanel.Visible = true;
            if(Position == World.GetEntityWithType<MPlayer>().Length || GetComponent<TransformComponent>().Location.Y < -150)
            {
                UIText text = gameOverPanel.GetText("GameOverText");
                text.Visible = true;
                //size (3,5)
                text.Size = new Vector2(1.5f, 2.5f);
                text.Position = new Vector2(-3.5f, 1f);
                text.Text = m_LastPlayerMessages[m_LastPlayerMessageIndex];
            }
            else
            {
                UIButton background = gameOverPanel.GetUIButton("Background");
                UIButtonImage imagButton = gameOverPanel.GetImageButton($"Medal{Position}");
                imagButton.Visible = true;
                if(Position == 1)
                {
                    background.Color = new Vector4(0.831f, 0.686f, 0.216f, 1);
                }
                else if(Position == 2)
                {
                    background.Color = new Vector4(1.0f, 1.0f, 1.0f, 0.863f);

                }
                else
                {
                    background.Color = new Vector4(0.804f, 0.498f, 0.196f, 0.863f);
                }
                switch (Position)
                {
                    case 1:
                    case 2:
                        {
                            if(imagButton.Position.Y <= 0)
                                break;
                            imagButton.Position = new Vector2(imagButton.Position.X, imagButton.Position.Y-(ts*1.3f));
                            break;
                        }
                    case 3:

                        {
                            if (imagButton.Position.Y <= 0.1)
                                break;
                            imagButton.Position = new Vector2(imagButton.Position.X, imagButton.Position.Y - (ts * 1.3f));
                            break;
                        }
                }
            }
        }
        bool m_InitalFinisehdLast = false;
        public bool Dead = false;
        void OnUpdate(float ts)
        {
            if (m_BlockOnUpdate)
            {
                SetInputState(PlayerInputState.Disabled);

                m_StartCountdown -= ts;
                if(m_StartCountdown < 0)
                    Countdown(ts);
                return;
            }
            m_ParticleSystem.GetParticle(0).Visible = false;
            UIPanel viewPanel = m_HUD.GetPanel(0);
            {
                float speed = GetComponent<TransformComponent>().Location.Z - m_lastPosZ;
                m_lastPosZ = GetComponent<TransformComponent>().Location.Z;
                //viewPanel.GetText("SpeedVal").Text = ((int)speed).ToString();
                viewPanel.GetText("SpeedVal").Text = Application.GetFPS().ToString("0.##");
                viewPanel.GetImageButton("Crosshair").Visible = false; 
            }
            if (GetComponent<TransformComponent>().Location.Y < -10000)
            {
                Dead = true;
                GetComponent<TransformComponent>().Location = new Vector3(0, -10000, 0);
            }
            if (Finished == true)
            {
                if (GetComponent<TransformComponent>().Location.Y >= -100)
                    m_InitalFinisehdLast = true;
                if (m_InitalFinisehdLast)
                    GetComponent<TransformComponent>().Location = new Vector3(GetComponent<TransformComponent>().Location.X, -30, GetComponent<TransformComponent>().Location.Z);
                GameOver(ts);
                return;
            }
            if (Dead)
                return;
            GetComponent<TransformComponent>().Rotation = new Vector3(0, 0, 0);

            ApplyCameraRotate();
            //{
            //    if (m_CameraEntity == null) return;
            //    float xOffset = Mouse.GetPositon().X - m_MouseLastPositon.X;
            //    float yOffset = m_MouseLastPositon.Y - Mouse.GetPositon().Y;
            //
            //    Vector3 rotation = m_CameraEntity.GetComponent<TransformComponent>().Rotation;
            //
            //    xOffset *= m_MouseSensitivity * World.GetTimeStep();
            //    m_Pitch *= m_MouseSensitivity * World.GetTimeStep();
            //    if (m_Pitch > 90)
            //        m_Pitch = 89;
            //    if (m_Pitch < -90)
            //        m_Pitch = -89;
            //    {
            //        rotation.X = (float)Math.Cos(ConvertDegreesToRadians(m_Yaw)) * (float)Math.Cos(ConvertDegreesToRadians(m_Pitch));
            //        rotation.Y = (float)Math.Sin(ConvertDegreesToRadians(m_Pitch));
            //        rotation.Z = (float)Math.Sin(ConvertDegreesToRadians(m_Yaw)) * (float)Math.Cos(ConvertDegreesToRadians(m_Pitch));
            //        //m_CameraEntity.GetComponent<TransformComponent>().Rotation = new Vector3(m_CameraEntity.GetComponent<TransformComponent>().Rotation.X, m_CameraEntity.GetComponent<TransformComponent>().Rotation.Y,
            //        //rotation.Z);
            //        m_CameraEntity.GetComponent<TransformComponent>().Rotation = rotation;
            //        m_MouseLastPositon = Mouse.GetPositon();
            //    }
            //}
           
            if (StartFinishCountdown)
            {
                if (m_FinishCountdown <= 0)
                    Finished = true;
                {
                    UIPanel countdownPanel = m_HUD.GetPanel(1);
                    countdownPanel.Visible = true;

                    countdownPanel.GetText("CountdownName").Text = "Over: ";
                    countdownPanel.GetText("CountdownName").Size = new Vector2(1.2f, 1.5f);
                    countdownPanel.GetText("CountdownName").Position = new Vector2(-1.8f, 3f);
                    UIText text = countdownPanel.GetText("Countdown");
                    text.Text = m_FinishCountdown.ToString("0.#");
                    text.Position = new Vector2(-0.5f, 3);
                    text.Size = new Vector2(2,1.5f);
                    text.Color = new Vector4(1,0,0,1);
                    if (Finished == true)
                    {
                        countdownPanel.Visible = false;

                    }
                }
                m_FinishCountdown -= ts;
            }
            // UpdateCamera(ts);
            if (m_TextEntity != null)
            {
                m_TextEntity.GetComponent<TextComponent>().Text = Position.ToString();
            }
           
            {
                for(int i =0; i< 2; i++)
                {
                    UIButtonImage slot= viewPanel.GetImageButton($"Inventory{i}");
                    if (m_Inventory[i] != null)
                    {
                        slot.Image = m_Inventory[i].Image;
                    }
                    else
                    {
                        slot.Image =new Texture( 0);
                    }

                }
            }

            if (m_RigidBody == null || DisableFowardForce == true) return;
            Vector3 velocity = new Vector3(m_RigidBody.GetLinearVelocity().X, m_RigidBody.GetLinearVelocity().Y,FowardForce* ts);
            m_RigidBody.SetLinearVelocity(velocity);
           // m_RigidBody.SetLinearVelocity(m_RigidBody.GetLinearVelocity()*0.095f);// add bit of friction
            if (m_IncreaseSpeedChange == true)
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
                //m_RigidBody.SetLinearVelocity(new Vector3(m_RigidBody.GetLinearVelocity().X, m_RigidBody.GetLinearVelocity().Y - 8, m_RigidBody.GetLinearVelocity().Z));
            }
            if (m_Inventory[m_CurrentInventory] != null)
            {
                if(m_Inventory[m_CurrentInventory].As<OpponentPowerUp>() != null)
                {
                    viewPanel.GetImageButton("Crosshair").Visible = true;
                }
            }
            int index = 0;
            foreach(PowerUp powerUp in m_Inventory)
            {
                UIText inventorySlot = viewPanel.GetText($"Inventory{index}");
                inventorySlot.Visible = false;

                if (powerUp == null)
                {
                    index++;
                    continue;
                }

                PlayerPowerUp playerPowerUp = powerUp.As<PlayerPowerUp>();
                if (playerPowerUp == null)
                {
                    index++;
                    continue;
                }

                if (powerUp.HasBeenThrown())
                {
                    
                    if (playerPowerUp.GetCountDown() <=0.0f)
                    {
                        m_Inventory[index] = null;
                        inventorySlot.Visible = false;
                    }
                    else
                    {
                        inventorySlot.Visible = true;
                        float contdown = playerPowerUp.GetCountDown();
                        inventorySlot.Text = contdown.ToString("0.#");
                    }

                }
                index++;
            }

            foreach(ParticleSystem particle in m_Particles)
            {
                if (particle == null) continue;
                //particle.Visible = false;
            }
        }

        void MoveX(float motionValue)
        {
            if (Dead)
                return;
                Vector3 velocity = new Vector3(SideWayForce*World.GetTimeStep()*motionValue, m_RigidBody.GetLinearVelocity().Y, m_RigidBody.GetLinearVelocity().Z);

           // m_RigidBody.SetLinearVelocity(velocity);

            m_RigidBody.AddForce(new Vector3(SideWayForce * World.GetTimeStep()* motionValue, 0, 0), ForceMode.VelocityChange);
        }

        // vaue (0,1)
        void IncreaseSpeed(float motionValue)
        {

            if (Dead)
                return;
            float value = Math.Abs(motionValue);
            m_IncreaseSpeedValue = value * m_FowardForceChange;
            FowardForce  += m_IncreaseSpeedValue;
            m_IncreaseSpeedChange = true;
            m_ParticleSystem.GetParticle(0).Visible = true;
        }
        void DecreaseSpeed(float motionValue)
        {

            if (Dead)
                return;
            float value = Math.Abs(motionValue);
            m_DecreaseSpeedValue = value * m_FowardForceChange;
            FowardForce -= m_DecreaseSpeedValue;
            m_DecreaseSpeedChange = true;

        }
        void Jump()
        {

            if (Dead)
                return;
            if (m_Jumping)
                return;
            m_Jumping = true;
            m_TouchingPlane = false;
            m_RigidBody.AddForce(new Vector3(0, 100,0), ForceMode.Impule);
        }
        void OnCollisionEnter(Entity otherEntity)
        { 
            
            if (Dead)
                return;
            List<string> namesTolookfor = new List<string>();
            namesTolookfor.Add("plane");
            namesTolookfor.Add("RampObstacle");
            namesTolookfor.Add("physicsCube");
            //namesTolookfor.Add("mObstacle");
            if (namesTolookfor.Contains(otherEntity.Name))
            {
                if (m_Jumping == true)
                {
                    m_TouchingPlane = true;
                    m_Jumping = false;
                   // m_RigidBody.SetLinearVelocity(new Vector3(m_RigidBody.GetLinearVelocity().X, 0, m_RigidBody.GetLinearVelocity().Z));

                }
                return;
            }

            if(otherEntity.Name == "finishLevel")
            {
                Finished = true;
                UIPanel viewPanel = m_HUD.GetPanel(0);
                viewPanel.Visible = false;
                //touched the plan first
                if (Position -1  == World.TryFindEntityByTag("GameManager").As<mGameManager>().CurrentPositon)
                 World.TryFindEntityByTag("GameManager").As<mGameManager>().CurrentPositon = Position;
                else if(Position == World.TryFindEntityByTag("GameManager").As<mGameManager>().CurrentPositon)
                    World.TryFindEntityByTag("GameManager").As<mGameManager>().CurrentPositon = Position;
                else
                {
                    World.TryFindEntityByTag("GameManager").As<mGameManager>().CurrentPositon += 1;
                    Position -= 1;
                }
                m_RigidBody.SetLinearVelocity(new Vector3(0));
                SetInputState(PlayerInputState.Disabled);
                MPlayer[] players = World.GetEntityWithType<MPlayer>();
                if (players.Length == Position)
                    m_Last = true;
            }
        }
        void ApplyPowerUp()
        {

            if (Dead)
                return;
            if (m_Inventory[m_CurrentInventory] != null)
            {
                m_Inventory[m_CurrentInventory].Throw();
                PowerUp powerUp = m_Inventory[m_CurrentInventory];
                if(powerUp.As<PlayerPowerUp>() == null)
                    m_Inventory[m_CurrentInventory] = null;
            }
        }
        void Drop()
        {

            if (Dead)
                return;
            if (m_Inventory[m_CurrentInventory] != null)
            {
                m_Inventory[m_CurrentInventory].Drop();
                m_Inventory[m_CurrentInventory] = null;
                Log.Info("Drop");
            }
        }
        void InCreaseInventory()
        {

            if (Dead)
                return;
            UIPanel viewPanel = m_HUD.GetPanel(0);
            UIButton inventoryIndicatorslot = viewPanel.GetUIButton($"InventoryIndicator{m_CurrentInventory}");
            inventoryIndicatorslot.Visible = false;
            if (m_CurrentInventory == 1)
                m_CurrentInventory = 0;
            else
                m_CurrentInventory++;
            
            inventoryIndicatorslot = viewPanel.GetUIButton($"InventoryIndicator{m_CurrentInventory}");
            inventoryIndicatorslot.Visible = true;
            Log.Info($"Increase Inventory crruent invenotry {m_CurrentInventory}");
        }

        void DecreaseInventory()
        {

            if (Dead)
                return;
            UIPanel viewPanel = m_HUD.GetPanel(0);
            UIButton inventoryIndicatorslot = viewPanel.GetUIButton($"InventoryIndicator{m_CurrentInventory}");
            inventoryIndicatorslot.Visible = false;
            if (m_CurrentInventory == 0)
                m_CurrentInventory = 1;
            else
                m_CurrentInventory--;
            inventoryIndicatorslot = viewPanel.GetUIButton($"InventoryIndicator{m_CurrentInventory}");
            inventoryIndicatorslot.Visible = true;
            Log.Info($"Decrease Inventory crruent invenotry {m_CurrentInventory}");

        }
        void PickUp(PowerUp powerUp)
        {

            if (Dead)
                return;
            for (int i=0; i<m_Inventory.Length;i++)
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

            if (Dead)
                return;
            PowerUp powerUp = other.As<PowerUp>();
            if (powerUp != null)
            {
                if (powerUp.HasOwner())
                    return;
                PickUp(powerUp);
            }
        }

        public void RemoveItem(PowerUp power)
        {

            if (Dead)
                return;
            for (int i =0; i<m_Inventory.Length; i++)
            {
                if (m_Inventory[i] == power)
                    m_Inventory[i] = null;
            }
        }
    }
}
