#pragma once
#include "Proof/Core/Layer.h"
#include "glm/glm.hpp"
#include "Proof/ProofCore.h"

namespace Proof {
	class Proof_API Editore3D : public Layer {
	public:
		Editore3D();
		~Editore3D() {
			delete ActiveWorld;
		}
		virtual void OnUpdate(FrameTime DeltaTime) override;
		virtual void OnImGuiDraw() override;
		virtual void OnAttach() override;
	private:
		void NewWorld();

		bool Docking = false;
		void ViewPort();
		glm::vec2 _ViewPortSize;
		void SetDocking(bool *p_open);
		glm::vec2 _ViewPortLocation;
		bool ViewPortResize = false;
		class World* ActiveWorld;
		class OrthagraphicCamera SceneCamera{-1.0f,1.0f,-1.0f,1.0f};
		glm::vec3 Square1Pos={0,0,0};
		glm::vec3 SquareScale ={1.0f,1.0f,1.0f};
		glm::vec4 Square1Color = {1.0f,1.0f,1.0f,1.f};
		glm::vec3 Rotation = {0,0,0};
		Count<Texture2D> WoodenTexture;
	};
}