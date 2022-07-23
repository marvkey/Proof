#pragma once

namespace Proof
{
	class SceneCoreClasses {
	public:
		static class World* GetCurrentWorld();
		static class PhysicsEngine* GetCurrentPhysicsEngine();
	private:
		static class World* s_CurrentWorld;
		friend class Editore3D;
		friend class Application;
	};
}
