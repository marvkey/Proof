#pragma once

#include "Proof/Core/Core.h""
#include "Proof/Scene/Mesh.h"
namespace Proof {
	class MeshWorkShop {
	public:
		static const Count<Mesh>& GetCubeMesh(){
			return m_Cube;
		}
		static const Count<Mesh>& GetSphere() {
			return m_Sphere;
		}

		static const Count<Mesh>& GetCapsule() {
			return m_Capasule;
		}
	private:
		static void Init();
		static void InitCube();
		static void InitSphere();
		static void InitCapsule();
		static Count<Mesh> m_Cube;
		static Count<Mesh> m_Sphere;
		static Count<Mesh> m_Capasule;
		friend class Renderer;
	};
}