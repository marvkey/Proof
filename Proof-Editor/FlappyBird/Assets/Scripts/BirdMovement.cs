
using System;
using Proof;

namespace FlappyBird
{
	public class BirdMovement : Entity
	{
		
		PlayerInputComponent m_PlayerInputComponent;
		
		public enum BirdState
		{
			Playing,
			Dead
		};

		public UIPanel PlayerScorePanel;
		BirdState m_State = BirdState.Playing;

		public BirdState GetState()
		{
			return m_State;
		}
		uint m_Score = 0;
		public float ForwardSpeed = 2f;
		public float MaxForwardSpeed = 10;
		public float JumpForce = 5f;
		public float IncreaseSpeedIntervalSeconds = 5;
		float m_IncreaseSpeedTimer;
		public float IncreaseSpeedValue = 0.25f;
        public InputAction JumpAction;
        public InputAction ChangeCameraAction;
        RigidBodyComponent m_RigidBody;
		public Entity ScoreEntity;

		int m_CurrentCameraIndex = 0;
		// first camera is the default
		public Entity[] Cameras;
		public Entity[] TextCameras;
		PlayerHUDComponent m_HudComponent;
		// OnCreate is called once when the Entity that this script is attached to
		// is instantiated in the world at runtime
		void OnCreate()
		{
            m_PlayerInputComponent = GetComponent<PlayerInputComponent>();
            m_HudComponent = GetComponent<PlayerHUDComponent>();
			m_HudComponent.SetPanel(PlayerScorePanel);
            m_RigidBody = GetComponent<RigidBodyComponent>();
            TriggerEnterEvent += OnTriggerEnter;
            CollisionEnterEvent += OnCollisionEnter;
            m_RigidBody.LinearVelocity = new Vector3(-ForwardSpeed,0,0);

            if (JumpAction != null ) 
				m_PlayerInputComponent.BindAction(JumpAction, InteractionEvent.Completed, PlayerJump);
			if (ChangeCameraAction != null)
				m_PlayerInputComponent.BindAction(ChangeCameraAction, InteractionEvent.Completed, ChangeCamera);

			m_IncreaseSpeedTimer = IncreaseSpeedIntervalSeconds;
            m_HudComponent.SetText("0");
        }

        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
		{
			if(m_RigidBody == null) return;

			// ScoreEntity.GetComponent<TextComponent>().Text = m_Score.ToString();
			/// TextCameras[m_CurrentCameraIndex].GetComponent<TextComponent>().Text = m_Score.ToString();

			m_IncreaseSpeedTimer -= deltaTime;
			if(m_IncreaseSpeedTimer <=0)
			{
				IncreaseSpeed();
				m_IncreaseSpeedTimer = IncreaseSpeedIntervalSeconds;
            }

        }

		void IncreaseSpeed()
		{
			ForwardSpeed += IncreaseSpeedValue;
			JumpForce += IncreaseSpeedValue;
        }
        void OnTriggerEnter(Entity other)
		{
			if (other.Name == "ScoreIncreaseCollider")
			{
				m_Score++;
                string textOutput = m_Score.ToString();

                m_HudComponent.SetText(textOutput);
                Log.Info($"Player Score now {m_Score}");
			}
		}

		void OnCollisionEnter(Entity other)
		{
			if(other.Name == "Obstacle")
			{
				m_State = BirdState.Dead;
                Log.Info("Dead");
            }
        }

		void PlayerJump(InputActionOutput actionOutput)
		{
			switch(m_State)
			{
				case BirdState.Playing:
					m_RigidBody.AddForce(Mathf.Up * JumpForce, ForceMode.Impulse);
                    m_RigidBody.LinearVelocity = new Vector3(-ForwardSpeed, 0, 0);
					break;
            }
          
        }

		void ChangeCamera(InputActionOutput actionOutput)
		{
			if (Cameras.Length == 0)
				return;
			switch (m_State)
			{
				case BirdState.Playing:
					int previousCameraIndex = m_CurrentCameraIndex;

					m_CurrentCameraIndex++;

					if (m_CurrentCameraIndex == Cameras.Length)
					{
						m_CurrentCameraIndex = 0;
					}

					//TextCameras[previousCameraIndex].GetComponent<TextComponent>().Visible = false;
					Cameras[previousCameraIndex].GetComponent<CameraComponent>().ActiveForRendering = false;
                    Cameras[m_CurrentCameraIndex].GetComponent<CameraComponent>().ActiveForRendering = true;
                    //TextCameras[m_CurrentCameraIndex].GetComponent<TextComponent>().Visible = true;
                    break;
			}

		}

    }
}
