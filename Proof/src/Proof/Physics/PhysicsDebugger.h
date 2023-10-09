#pragma once
#include <PxPhysicsAPI.h>
namespace Proof
{
	class PhysicsDebugger
	{
	public:
		static void StartDebugging(const std::string& filepath, bool networkDebugging = false);
		static bool IsDebugging();
		static void StopDebugging();

		static physx::PxPvd* GetDebugger();
	private:
		static void Initialize();
		static void Shutdown();
		friend class PhysicsEngine;
	};
}