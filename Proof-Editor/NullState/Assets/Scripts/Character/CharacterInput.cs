

using System;
using Proof;

namespace NullState
{
    public class CharacterInput : Entity
    {
        public InputAction MoveAction;
        public InputAction RotateAction;
        public InputAction JumpAction;
        public InputAction IsRunningAction;

        PlayerInputComponent m_PlayerInputComponent;
        CharacterMovement m_PlayerMovement;

        void OnCreate()
        {
            m_PlayerInputComponent = GetComponent<PlayerInputComponent>();
            if (m_PlayerInputComponent == null)
            {
                Log.Error($"{Name} PlayerInput Script needs a PlayerInputComponent");
                return;
            }
            BindInputAction(m_PlayerInputComponent, MoveAction, InteractionEvent.Triggered, Move);
            BindInputAction(m_PlayerInputComponent, RotateAction, InteractionEvent.Triggered, Rotate);
            BindInputAction(m_PlayerInputComponent, JumpAction, InteractionEvent.Triggered, Jump);

            BindInputAction(m_PlayerInputComponent, IsRunningAction, InteractionEvent.Started, EnableRunning);
            BindInputAction(m_PlayerInputComponent, IsRunningAction, InteractionEvent.Completed, DisableRunning);

            m_PlayerMovement = GetScript<CharacterMovement>();

            Mouse.SetCursorMode(MouseCursorMode.Locked);
        }

        void EnableRunning(InputActionOutput actionOutput)
        {
            if (m_PlayerMovement != null)
                m_PlayerMovement.IsRunning = true;

        }

        void DisableRunning(InputActionOutput actionOutput)
        {
            if (m_PlayerMovement != null)
                m_PlayerMovement.IsRunning = false;

        }


        void Move(InputActionOutput actionOutput)
        {

            if (m_PlayerMovement != null)
            {
                m_PlayerMovement.Move(actionOutput.Get<Vector2>());
            }
        }

        void Rotate(InputActionOutput actionOutput)
        {
            if (m_PlayerMovement != null)
            {
                m_PlayerMovement.Rotate(actionOutput.Get<Vector2>());
            }
        }

        void Jump(InputActionOutput actionOutput)
        {
            if (m_PlayerMovement != null)
            {
                m_PlayerMovement.Jump();
            }
        }

    }
}
