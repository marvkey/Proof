
using System;
using System.Runtime.InteropServices;
using System.Threading;
using Proof;
using static FlappyBird.BirdMovement;

namespace FlappyBird
{
	public class BirdMovement : Entity
	{
		public struct TestData
		{
			int score;
		}
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
		public float AccelerationRate = 0.2f;
        public InputAction JumpAction;
        public InputAction ChangeCameraAction;
        RigidBodyComponent m_RigidBody;
		public Entity ScoreEntity;

		int m_CurrentCameraIndex = 0;
		public Prefab FracturedBird;
		// first camera is the default
		public Entity[] Cameras;
		public Entity[] TextCameras;
		PlayerHUDComponent m_HudComponent;
		private float m_WorldEndTimer = 5.0f;// seconds
        public UIPanel UIPanel;

        [StructLayout(LayoutKind.Sequential)]

        public struct PlayerData
        {
            public int Score;
            public float Time;
        }
        private byte[] StructToByteArray<T>(T structData) where T : struct
        {
            int size = Marshal.SizeOf(typeof(T));
            byte[] byteArray = new byte[size];

            IntPtr ptr = Marshal.AllocHGlobal(size);
            try
            {
                Marshal.StructureToPtr(structData, ptr, true);
                Marshal.Copy(ptr, byteArray, 0, size);
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }

            return byteArray;
        }
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
		{
            m_PlayerInputComponent = GetComponent<PlayerInputComponent>();
            m_HudComponent = GetComponent<PlayerHUDComponent>();
            m_RigidBody = GetComponent<RigidBodyComponent>();
            TriggerEnterEvent += OnTriggerEnter;
            CollisionEnterEvent += OnCollisionEnter;
            m_RigidBody.LinearVelocity = new Vector3(-ForwardSpeed,0,0);

            if (JumpAction != null ) 
				m_PlayerInputComponent.BindAction(JumpAction, InteractionEvent.Completed, PlayerJump);
			if (ChangeCameraAction != null)
				m_PlayerInputComponent.BindAction(ChangeCameraAction, InteractionEvent.Completed, ChangeCamera);

            PlayerData playerData = new PlayerData { Score = 100, Time = 12.5f };

			PersistentDataStorage.SaveData("Score", 10);

			//PersistentDataStorage.SaveDataRaw("Score", StructToByteArray(playerData));
        }

        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
		{
			if(m_RigidBody == null) return;

            // ScoreEntity.GetComponent<TextComponent>().Text = m_Score.ToString();
            /// TextCameras[m_CurrentCameraIndex].GetComponent<TextComponent>().Text = m_Score.ToString();

            if (m_State == BirdState.Playing && ForwardSpeed < MaxForwardSpeed)
            {
                ForwardSpeed += AccelerationRate * deltaTime;
                JumpForce += AccelerationRate * deltaTime; // optional scaling
            }
            RotateBird();

			if (m_State == BirdState.Dead)
				m_WorldEndTimer -= deltaTime;

			if (m_WorldEndTimer <= 0)
				World.Restart();
        }
     
        void OnTriggerEnter(Entity other)
		{
			if (other.Name == "ScoreIncreaseCollider")
			{
				m_Score++;
                string textOutput = m_Score.ToString();

                Variable var = m_HudComponent.GetRegistryVariable(3, UIPanel, "Score");
				var.SetData(textOutput);

                Log.Info($"Player Score now {m_Score}");
			}
		}

		void Fracture()
		{
			if (FracturedBird == null)
				return;

			Entity fractureBirdEntity = World.Instantiate(FracturedBird, GetComponent<RigidBodyComponent>().Location);

			/*
			fractureBirdEntity.Scale = Scale;

			Log.Info($"{fractureBirdEntity.Location}");
			Log.Info($"{Location}");
            foreach (Entity child in fractureBirdEntity.GetChildren())
			{
				//Vector3 impulse = Proof.Random.Vector(0,2);

				//child.GetComponent<RigidBodyComponent>().LinearVelocity = GetComponent<RigidBodyComponent>().LinearVelocity;
				//child.GetComponent<RigidBodyComponent>().AddForce(impulse, ForceMode.Impulse);
            }
			*/
			//Destroy();
		}
		void OnCollisionEnter(Entity other)
		{
			if (m_State == BirdState.Dead)
				return;

			if(other.Name == "Obstacle")
			{
				m_State = BirdState.Dead;
                Log.Info("Dead");
				//World.Restart();
				Fracture();
				GetComponent<MeshComponent>().Visible = false;
				GetComponent<BoxColliderComponent>().IsTrigger = true;
                m_RigidBody.LinearVelocity = new Vector3(0.0f);
                m_RigidBody.Gravity =false;
                m_RigidBody.IsKinematic = true;
				Log.Info("Stop player");
            }
        }
        public float maxTiltAngle = 60f; // Maximum angle to tilt up or down
        public float tiltSmooth = 5f; // Speed of rotation smoothing

        void RotateBird()
		{
			switch (m_State)

			{
				case BirdState.Playing:
					// Calculate the target angle based on the y-velocity
					float targetAngle = Mathf.Clamp(-m_RigidBody.Velocity.y * maxTiltAngle / JumpForce, -maxTiltAngle, maxTiltAngle);

					// Set the x-axis rotation directly
					Rotation = Vector3.Lerp(Rotation, new Vector3(0, 0, targetAngle), tiltSmooth * World.GetDeltaTime());
					break;
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
