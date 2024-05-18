
using System;
using Proof;

namespace Driftwood
{
	public class PlayerInput : Entity
	{
		public InputAction MoveSideWaysAction;
		PlayerInputComponent m_PlayerInputComponent;
		PlayerMovement m_PlayerMovement;
		// OnCreate is called once when the Entity that this script is attached to
		// is instantiated in the world at runtime
		void OnCreate()
		{
			m_PlayerInputComponent = GetComponent<PlayerInputComponent>();	
			if(m_PlayerInputComponent ==null)
			{
				Log.Error($"{Name} PlayerInput Script needs a PlayerInputComponent");
				return;
			}

			if(MoveSideWaysAction != null)
			{
				m_PlayerInputComponent.BindAction(MoveSideWaysAction, InteractionEvent.Triggered, MoveSideWays);
			}

			m_PlayerMovement = this.As<PlayerMovement>();
		}

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
		}

		void MoveSideWays(InputActionOutput actionOutput)
		{
			if(m_PlayerMovement != null)
			{
				m_PlayerMovement.MoveSideways(actionOutput.Get<float>());
			}
		}
	}
}
