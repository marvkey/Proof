#pragma once

#include "Proof/Core/Core.h""
#include "Proof/Scene/Mesh.h"
namespace Proof {
	class MeshWorkShop {
	public:
		static Count<Mesh> GenerateCube();
		
	private:
		static void Init();
		static void InitCube();
		static void InitSphere();
		static void InitCapsule();
		friend class RendererBase;
	};
}