
using System;
using Proof;

namespace SandboxProject
{
	public class ScriptSpawner : Entity
	{
		public int AmountCubes;
		// OnCreate is called once when the Entity that this script is attached to
		// is instantiated in the world at runtime
		void OnCreate()
		{
			Log.Info($"AMount of cubes {AmountCubes}");
		}

		// OnUpdate is called once every frame while this script is active in the world
		void OnUpdate(float deltaTime)
		{
		}

	}
}
