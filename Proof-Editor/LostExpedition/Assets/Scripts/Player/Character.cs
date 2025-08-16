
using System;
using System.Runtime.InteropServices;
using Proof;
namespace LostExpedition
{
    class Character : Entity
    {
        public Character()
        {
            // Constructor logic if needed
        }
        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
        {
            Log.Info($"{Name} has been created.");
        }
        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
        {
            Log.Info($"{Name} is updating with delta time: {deltaTime}");
            // Update logic for the player can be added here
        }
    }
}
