#pragma once

#include "Proof/Core/Core.h""
#include "Proof/Scene/Mesh.h"
namespace Proof {
	class MeshWorkShop {
	public:
		static const Count<Mesh>& GetCubeMesh(){
			return m_Cube;
		}
	private:
		static void Init();
		static void InitCube();
		static Count<Mesh> m_Cube;
		friend class Renderer;
	};
}