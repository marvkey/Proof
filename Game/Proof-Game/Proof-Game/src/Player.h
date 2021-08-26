#pragma once
#include "generated/Init.h"
#include <Proof/Scene/Script.h>
#include <Proof/Scene/Material.h>
#include <Proof/Input/Input.h>
/*
Proof.lib
opengl32.lib
Glad.lib
yaml - cpp.lib
ImGui.lib
GLFW.lib
*/
namespace Proof{
	REGISTER_SCRIPT(Player)
	class ProofGAMEAPI Player: public Script {
	public:
		float Speed = 2.2;
		void OnUpdate(FrameTime DeltaTime) {
			if (Input::IsKeyPressed(KeyBoardKey::LeftArrow)) {
				GetComponent<TransformComponent>()->Location.X += Speed;
			}

			if (Input::IsKeyPressed(KeyBoardKey::RightArrow)) {
				GetComponent<TransformComponent>()->Location.X -= Speed;
			}

			if (Input::IsKeyPressed(KeyBoardKey::UpArrow)) {
				GetComponent<TransformComponent>()->Location.Z += Speed;
			}


			if (Input::IsKeyPressed(KeyBoardKey::DownArrow)) {
				GetComponent<TransformComponent>()->Location.Z -= Speed;
			}
			
			if(Input::IsKeyReleased(KeyBoardKey::G)){
				if(GetComponent<MeshComponent>() != nullptr){
					if(GetComponent<MeshComponent>()->HasMaterial()==true){
						GetComponent<MeshComponent>()->GetMaterial()->m_Ambient.x = Math::Sin(DeltaTime.GetTime() * 2.0f);
					}
				}
			}
			PF_ENGINE_ERROR("yohh gg wagwan %f",DeltaTime.GetTime());
		} 
		//virtual void OnCreate();
	};

	REGISTER_SCRIPT(PlayerController) 
	class ProofGAMEAPI PlayerController: public Script {
	public:
		void OnUpdate(FrameTime DeltaTime) {
			std::cout << "wagawan innit";
		}
	};
}
 