
using System;
using System.Runtime.InteropServices;
using Proof;

namespace Proof
{
    public class CharacterInput : Entity
    {
        public InputAction MoveAction;
        public InputAction RotateAction;
        public InputAction JumpAction;
        public InputAction ActivateLightAction;
        public InputAction IsRunningAction;

        public Entity Flashlight;

        PlayerInputComponent m_PlayerInputComponent;
        CharacterMovement m_PlayerMovement;
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
        {
            m_PlayerInputComponent = GetComponent<PlayerInputComponent>();
            if (m_PlayerInputComponent == null)
            {
                Log.Error($"{Name} PlayerInput Script needs a PlayerInputComponent");
                return;
            }

            if (MoveAction != null)
            {
                m_PlayerInputComponent.BindAction(MoveAction, InteractionEvent.Triggered, Move);
            }

            if (RotateAction != null)
            {
                m_PlayerInputComponent.BindAction(RotateAction, InteractionEvent.Triggered, Rotate);
            }

            if (JumpAction != null)
            {
                m_PlayerInputComponent.BindAction(JumpAction, InteractionEvent.Triggered, Jump);
            }

          


            BindInputAction(m_PlayerInputComponent, ActivateLightAction, InteractionEvent.Triggered, ActivateLight);
            BindInputAction(m_PlayerInputComponent, IsRunningAction, InteractionEvent.Started, EnableRunning);
            BindInputAction(m_PlayerInputComponent, IsRunningAction, InteractionEvent.Completed, DisableRunning);

            m_PlayerMovement = GetScript<CharacterMovement>();

        }

        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
        {
           
        }

        // OnPhysicsUpdate is called at a fixed timestep for physics-related logic
        void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
        {

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

        void ActivateLight(InputActionOutput actionOutput)
        {
            if(Flashlight != null)
                Flashlight.GetComponent<SpotLightComponent>().Intensity = 1.0f;
        }
    }
}
