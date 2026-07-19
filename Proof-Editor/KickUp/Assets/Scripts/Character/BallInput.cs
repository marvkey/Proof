using Proof;

namespace KickUp
{
    public class BallInput : Entity
    {
        [Header("Input Actions")]
        public InputAction CameraAction;
        public InputAction LoftAction;
        public InputAction LoftScrollAction;
        public InputAction KickAction;
        public InputAction BrakeAction;
        public InputAction UnlockCursorAction;

        public float LoftChangeSpeed = 1.5f;
        public float ScrollLoftStep = 0.08f;

        private PlayerInputComponent m_PlayerInputComponent;
        private BallMovement m_BallMovement;
        private CameraMovement m_CameraMovement;

        void OnCreate()
        {
            m_PlayerInputComponent = GetComponent<PlayerInputComponent>();

            if (m_PlayerInputComponent == null)
            {
                Log.Error($"{Name}: BallInput requires a PlayerInputComponent.");
                return;
            }

            m_BallMovement = GetScript<BallMovement>();
            m_CameraMovement = GetScript<CameraMovement>();

            if (m_BallMovement == null)
                Log.Error($"{Name}: BallInput could not find BallMovement.");

            if (m_CameraMovement == null)
                Log.Error($"{Name}: BallInput could not find CameraMovement.");

            BindInputActions();

            Mouse.SetCursorMode(MouseCursorMode.Locked);
        }

        private void BindInputActions()
        {
            // Mouse movement.
            BindInputAction(
                m_PlayerInputComponent,
                CameraAction,
                InteractionEvent.Triggered,
                MoveCamera
            );

            // W/S or Up/Down held input.
            BindInputAction(
                m_PlayerInputComponent,
                LoftAction,
                InteractionEvent.Triggered,
                ChangeLoft
            );

            // Mouse-wheel input.
            BindInputAction(
                m_PlayerInputComponent,
                LoftScrollAction,
                InteractionEvent.Triggered,
                ChangeLoftWithScroll
            );

            // Left mouse button.
            BindInputAction(
                m_PlayerInputComponent,
                KickAction,
                InteractionEvent.Started,
                StartKickCharge
            );

            BindInputAction(
                m_PlayerInputComponent,
                KickAction,
                InteractionEvent.Completed,
                ReleaseKick
            );

            // Right mouse button.
            BindInputAction(
                m_PlayerInputComponent,
                BrakeAction,
                InteractionEvent.Started,
                EnableBrake
            );

            BindInputAction(
                m_PlayerInputComponent,
                BrakeAction,
                InteractionEvent.Completed,
                DisableBrake
            );

            // Escape.
            BindInputAction(
                m_PlayerInputComponent,
                UnlockCursorAction,
                InteractionEvent.Started,
                UnlockCursor
            );
        }

        private void MoveCamera(InputActionOutput actionOutput)
        {
            if (m_CameraMovement == null)
                return;

            Vector2 cameraInput = actionOutput.Get<Vector2>();

            m_CameraMovement.AddCameraInput(
                cameraInput.x,
                cameraInput.y
            );
        }

        private void ChangeLoft(InputActionOutput actionOutput)
        {
            if (m_BallMovement == null)
                return;

            float input = actionOutput.Get<float>();

            m_BallMovement.AddLoft(
                input * LoftChangeSpeed * World.GetDeltaTime()
            );
        }

        private void ChangeLoftWithScroll(InputActionOutput actionOutput)
        {
            if (m_BallMovement == null)
                return;

            float scroll = actionOutput.Get<float>();

            m_BallMovement.AddLoft(
                scroll * ScrollLoftStep
            );
        }

        private void StartKickCharge(InputActionOutput actionOutput)
        {
            if (m_BallMovement == null)
                return;

            m_BallMovement.StartCharge();
        }

        private void ReleaseKick(InputActionOutput actionOutput)
        {
            if (m_BallMovement == null)
                return;

            m_BallMovement.ReleaseKick();
        }

        private void EnableBrake(InputActionOutput actionOutput)
        {
            if (m_BallMovement != null)
                m_BallMovement.SetBrake(true);
        }

        private void DisableBrake(InputActionOutput actionOutput)
        {
            if (m_BallMovement != null)
                m_BallMovement.SetBrake(false);
        }

        private void UnlockCursor(InputActionOutput actionOutput)
        {
            Mouse.SetCursorMode(MouseCursorMode.Normal);

            if (m_BallMovement != null)
                m_BallMovement.SetBrake(false);
        }
    }
}