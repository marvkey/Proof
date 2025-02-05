
using System;
using Proof;

namespace PacMan
{
	public class PlayerInput : Entity
	{
        public InputAction MovePlayerAction;
        PlayerMovement m_PlayerMovement;
        PlayerInputComponent m_PlayerInputComponent;


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

            if (MovePlayerAction != null)
            {
                m_PlayerInputComponent.BindAction(MovePlayerAction, InteractionEvent.Triggered, MovePlayer);
            }

            m_PlayerMovement = GetScript<PlayerMovement>();

        }

        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
		{
		}

        void MovePlayer(InputActionOutput actionOutput)
        {
            if (m_PlayerMovement == null)
                return;

            m_PlayerMovement.Move(actionOutput.Get<Vector2>());

        }


    }
}
