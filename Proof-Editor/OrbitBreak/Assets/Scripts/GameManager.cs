using System;
using OrbitBreak;
using Proof;

namespace Proof
{
    class GameManager :Entity
    {
        public Entity Player;

        public GameHardState GameState
        {
            get
            {
                if(m_Counter > 30.0f)
                {
                    return GameHardState.Medium;
                }
                else if(m_Counter > 60.0f)
                {
                    return GameHardState.Hard;
                }
                else
                {
                    return GameHardState.Easy;
                }
            }
        }
        public enum GameHardState
        {
            Easy =0,
            Medium  =1,
            Hard = 2
        }

        float m_Counter;
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
        {

        }

        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
        {
            m_Counter = +deltaTime;

            
            UpdateUI();
        }

        // OnPhysicsUpdate is called at a fixed timestep for physics-related logic
        void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
        {
        }

        void UpdateUI()
        {
            if (Player == null)
            {
                GetComponent<PlayerHUDComponent>().GetRegistryVariableByPanelIndex(3, 0, "Health")?.SetData((0.0f));
                return;

            }
            float health = Player.GetScript<Player>().Health;

            GetComponent<PlayerHUDComponent>().GetRegistryVariableByPanelIndex(3,0,"Health")?.SetData((float)health /100.0f);
        }
    }
}
